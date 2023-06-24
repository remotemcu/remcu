
C_FILES  += $(addprefix -a ,$(INC_PATH))
PYTHON_HEADER_SCRIPT = $(CxxHeader2Py_PATH)/header2.py
PY_VERSION = --version=$(MCU_MAJOR_VERSION_LIB)-`git rev-parse --short HEAD`
NAME_PY_FILE = ${DEVICE}_$(MCU_LIB_NAME).py

python: $(NAME_PY_FILE)

$(NAME_PY_FILE):
	python $(PYTHON_HEADER_SCRIPT) -c "$(LOCAL_CFLAGS)" $(C_FILES) $(PY_VERSION) -o $(NAME_PY_FILE) -d 1