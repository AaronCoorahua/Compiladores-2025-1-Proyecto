import subprocess
import os
import time
from pathlib import Path
from PyQt5.QtWidgets import (
    QApplication, QWidget, QVBoxLayout, QPushButton, QTextEdit, QLabel, QMessageBox,
    QTreeView, QFileSystemModel, QHBoxLayout, QSplitter, QInputDialog, QPlainTextEdit,
    QFrame, QStackedWidget
)
from PyQt5.QtCore import Qt, QRect, QSize
from PyQt5.QtGui import QPainter, QColor, QFont, QTextFormat

class LineNumberArea(QWidget):
    def __init__(self, editor):
        super().__init__(editor)
        self.editor = editor

    def sizeHint(self):
        return QSize(self.editor.line_number_area_width(), 0)

    def paintEvent(self, event):
        self.editor.line_number_area_paint_event(event)

class CodeEditor(QPlainTextEdit):
    def __init__(self):
        super().__init__()
        self.line_number_area = LineNumberArea(self)
        self.blockCountChanged.connect(self.update_line_number_area_width)
        self.updateRequest.connect(self.update_line_number_area)
        self.cursorPositionChanged.connect(self.highlight_current_line)
        self.update_line_number_area_width(0)
        self.setStyleSheet("background-color: white; color: black;")
        self.setFont(QFont("Consolas", 12))
        self.setFrameStyle(QFrame.NoFrame)

    def line_number_area_width(self):
        digits = len(str(max(1, self.blockCount())))
        return 10 + self.fontMetrics().horizontalAdvance('9') * digits

    def update_line_number_area_width(self, _):
        self.setViewportMargins(self.line_number_area_width(), 0, 0, 0)

    def update_line_number_area(self, rect, dy):
        if dy:
            self.line_number_area.scroll(0, dy)
        else:
            self.line_number_area.update(0, rect.y(), self.line_number_area.width(), rect.height())
        if rect.contains(self.viewport().rect()):
            self.update_line_number_area_width(0)

    def resizeEvent(self, event):
        super().resizeEvent(event)
        cr = self.contentsRect()
        self.line_number_area.setGeometry(QRect(cr.left(), cr.top(), self.line_number_area_width(), cr.height()))

    def line_number_area_paint_event(self, event):
        painter = QPainter(self.line_number_area)
        painter.fillRect(event.rect(), QColor(245, 245, 245))
        block = self.firstVisibleBlock()
        block_number = block.blockNumber()
        top = int(self.blockBoundingGeometry(block).translated(self.contentOffset()).top())
        bottom = top + int(self.blockBoundingRect(block).height())
        while block.isValid() and top <= event.rect().bottom():
            if block.isVisible() and bottom >= event.rect().top():
                number = str(block_number + 1)
                painter.setPen(Qt.black)
                painter.drawText(0, top, self.line_number_area.width() - 5, self.fontMetrics().height(),
                                 Qt.AlignRight, number)
            block = block.next()
            top = bottom
            bottom = top + int(self.blockBoundingRect(block).height())
            block_number += 1

    def highlight_current_line(self):
        extra_selections = []
        if not self.isReadOnly():
            selection = QTextEdit.ExtraSelection()
            line_color = QColor(232, 242, 254)
            selection.format.setBackground(line_color)
            selection.format.setProperty(QTextFormat.FullWidthSelection, True)
            selection.cursor = self.textCursor()
            selection.cursor.clearSelection()
            extra_selections.append(selection)
        self.setExtraSelections(extra_selections)

class PascalFrontend(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Pascal IDE - PyQt")
        self.resize(1200, 700)
        self.current_file_path = None

        self.main_splitter = QSplitter(Qt.Horizontal)

        # Sidebar
        sidebar_widget = QWidget()
        sidebar_layout = QVBoxLayout()
        self.model = QFileSystemModel()
        self.project_dir = os.path.abspath("project/")
        os.makedirs(self.project_dir, exist_ok=True)
        self.model.setRootPath(self.project_dir)

        self.tree = QTreeView()
        self.tree.setModel(self.model)
        self.tree.setRootIndex(self.model.index(self.project_dir))
        self.tree.setHeaderHidden(True)
        self.tree.doubleClicked.connect(self.on_file_selected)

        for text, slot in [
            ("📄 Nuevo archivo", self.create_new_file),
            ("📁 Nueva carpeta", self.create_new_folder),
            ("🗑️ Eliminar archivo", self.delete_selected),
            ("🗑️ Eliminar carpeta", self.delete_selected_folder),
        ]:
            btn = QPushButton(text)
            btn.clicked.connect(slot)
            sidebar_layout.addWidget(btn)
        sidebar_layout.addWidget(self.tree)
        sidebar_widget.setLayout(sidebar_layout)

        # Centro: Editor
        self.center_widget = QWidget()
        center_layout = QVBoxLayout()
        self.editor = CodeEditor()
        self.editor.setPlaceholderText("Escribe tu código Pascal aquí...")
        btn_run = QPushButton("Run (Generar ASM)")
        btn_run.clicked.connect(self.build_and_run_compiler)
        btn_exec = QPushButton("Ejecutar programa")
        btn_exec.clicked.connect(self.run_binary)
        center_layout.addWidget(self.editor, stretch=1)
        center_layout.addWidget(btn_run)
        center_layout.addWidget(btn_exec)
        self.center_widget.setLayout(center_layout)
        self.center_widget.setVisible(False)

        # Derecha: ASM + Output + Logo pequeño
        self.asm_text = QTextEdit()
        self.output_text = QTextEdit()
        self.asm_text.setReadOnly(True)
        self.output_text.setReadOnly(True)

        self.logo_label = QLabel()
        self.logo_label.setAlignment(Qt.AlignCenter)
        self.logo_label.setStyleSheet("font-family: monospace; font-size: 10px; color: gray;")
        logo_path = "project/build/logo.txt"
        if os.path.exists(logo_path):
            with open(logo_path, "r", encoding="utf-8") as f:
                self.logo_label.setText(f.read())
        else:
            self.logo_label.setText("⚠️ Logo no encontrado")

        output_layout = QVBoxLayout()
        output_layout.addWidget(QLabel("📄 Código ensamblador:"))
        output_layout.addWidget(self.asm_text)
        output_layout.addWidget(QLabel("💻 Salida del programa:"))
        output_layout.addWidget(self.output_text)
        output_layout.addWidget(self.logo_label)
        output_panel = QWidget()
        output_panel.setLayout(output_layout)

        self.right_panel = QStackedWidget()
        self.right_panel.addWidget(self.build_logo_start())
        self.right_panel.addWidget(output_panel)
        self.right_panel.setCurrentIndex(0)

        self.main_splitter.addWidget(sidebar_widget)
        self.main_splitter.addWidget(self.center_widget)
        self.main_splitter.addWidget(self.right_panel)
        self.main_splitter.setStretchFactor(0, 2)
        self.main_splitter.setStretchFactor(1, 6)
        self.main_splitter.setStretchFactor(2, 2)

        layout = QHBoxLayout()
        layout.addWidget(self.main_splitter)
        self.setLayout(layout)

    def build_logo_start(self):
        logo = QLabel()
        logo.setAlignment(Qt.AlignCenter)
        logo.setStyleSheet("font-family: monospace; font-size: 14px;")
        logo_path = "project/build/logo.txt"
        if os.path.exists(logo_path):
            with open(logo_path, "r", encoding="utf-8") as f:
                logo.setText(f.read())
        else:
            logo.setText("⚠️ Logo no encontrado")
        layout = QVBoxLayout()
        layout.addStretch()
        layout.addWidget(logo)
        layout.addStretch()
        widget = QWidget()
        widget.setLayout(layout)
        return widget

    def create_new_file(self):
        name, ok = QInputDialog.getText(self, "Nuevo archivo", "Nombre del archivo:")
        if ok and name:
            path = os.path.join(self.project_dir, name)
            with open(path, "w") as f:
                f.write("")
            self.model.setRootPath("")
            self.model.setRootPath(self.project_dir)

    def create_new_folder(self):
        name, ok = QInputDialog.getText(self, "Nueva carpeta", "Nombre de la carpeta:")
        if ok and name:
            os.makedirs(os.path.join(self.project_dir, name), exist_ok=True)
            self.model.setRootPath("")
            self.model.setRootPath(self.project_dir)

    def delete_selected(self):
        index = self.tree.currentIndex()
        path = self.model.filePath(index)
        if os.path.isfile(path):
            os.remove(path)
            self.model.setRootPath("")
            self.model.setRootPath(self.project_dir)

    def delete_selected_folder(self):
        index = self.tree.currentIndex()
        path = self.model.filePath(index)
        if os.path.isdir(path):
            try:
                os.rmdir(path)
                self.model.setRootPath("")
                self.model.setRootPath(self.project_dir)
            except OSError:
                QMessageBox.warning(self, "Error", "La carpeta no está vacía.")

    def on_file_selected(self, index):
        path = self.model.filePath(index)
        if os.path.isfile(path):
            with open(path, "r", encoding="utf-8") as f:
                self.editor.setPlainText(f.read())
            self.current_file_path = path
            self.center_widget.setVisible(True)
            self.right_panel.setCurrentIndex(1)

            # Ajustar proporciones 20% - 60% - 20%
            total_width = self.main_splitter.width()
            sidebar_w = int(total_width * 0.2)
            center_w = int(total_width * 0.6)
            right_w = total_width - sidebar_w - center_w
            self.main_splitter.setSizes([sidebar_w, center_w, right_w])

    def build_and_run_compiler(self):
        try:
            if self.current_file_path:
                with open(self.current_file_path, "w", encoding="utf-8") as f:
                    f.write(self.editor.toPlainText())

            os.makedirs("build", exist_ok=True)
            subprocess.run(["cmake", ".."], cwd="build", check=True)
            subprocess.run(["cmake", "--build", "."], cwd="build", check=True)

            input_name = os.path.basename(self.current_file_path)
            temp_input_path = os.path.join("project", input_name)
            output_s_path = os.path.join("project", "build", input_name.replace(".txt", ".s"))

            exe_path = "build/Debug/Proyecto.exe" if os.name == 'nt' else "./build/Proyecto"
            subprocess.run([exe_path, temp_input_path, "--frontend"], check=True)

            timeout, elapsed = 5, 0
            while not os.path.exists(output_s_path) and elapsed < timeout:
                time.sleep(0.1)
                elapsed += 0.1

            if os.path.exists(output_s_path):
                with open(output_s_path, "r", encoding="utf-8") as f:
                    self.asm_text.setPlainText(f.read())
            else:
                self.asm_text.setPlainText("❌ No se encontró el archivo ensamblador.")
        except subprocess.CalledProcessError as e:
            self.asm_text.setPlainText("❌ Error en compilación:\n" + str(e))
        except Exception as e:
            self.asm_text.setPlainText("❌ Error inesperado:\n" + str(e))

    def run_binary(self):
        try:
            stem = Path(self.current_file_path).stem
            asm_file = os.path.join("project", "build", f"{stem}.s")
            cmd = [
                "docker", "run", "--rm",
                "-v", os.path.abspath(".") + ":/app",
                "-w", "/app",
                "pascal-linux-runner",
                "bash", "-c",
                f"gcc -no-pie '{asm_file.replace(os.sep, '/')}' -o programa && ./programa"
            ]
            result = subprocess.run(cmd, capture_output=True, text=True)
            self.output_text.setPlainText(result.stdout if result.returncode == 0 else "❌ Error en ejecución:\n" + result.stderr)
        except Exception as e:
            self.output_text.setPlainText("❌ Error inesperado:\n" + str(e))

if __name__ == "__main__":
    app = QApplication([])
    win = PascalFrontend()
    win.show()
    app.exec()
