# Python

<p align="center">
  <a href="README.md">English</a> |
  <span>Русский</span> |
  <a href="README.zh-CN.md">简体中文</a> |
  <a href="README.de-DE.md">Deutsch</a> |
  <a href="README.fr-FR.md">Français</a> |
  <a href="README.es-ES.md">Español</a> |
  <a href="README.ko-KR.md">한국어</a>
</p>

Этот модуль — альтернатива управлению транспортным средством OpenBot со смартфона на базе встроенного Linux. Написанный на Python, OpenBot может управляться с помощью компьютера под управлением Linux и камеры для восприятия окружения.

Роботом можно управлять двумя способами: через инференс политики нейронной сети или с помощью джойстика.

```
├── __init__.py
├── README.md
├── requirements.txt
├── run.py
├── generate_data_for_training.py
├── export_openvino.py
├── infer.py
├── joystick.py
├── realsense.py
└── tests
    ├── test_data
    │   └── logs1
    │       └── ...
    ├── test_models
    │   ├── openvino
    │   ├── tflite
    │   └── tf.zip
    ├── test_export_openvino.py
    ├── test_infer.py
    ├── test_joystick.py
    ├── test_motor.py
    └── test_realsense.py

```
## Запуск робота

Для работы робота запустите `run.py` — главный Python-скрипт. Робота можно запускать в 3 режимах:
- Debug (отладка): в этом режиме политика выполняется в автономном (off-line) режиме. То есть вместо реальных изображений с камеры и команд ввода с джойстика используются данные (команды и изображения), загруженные из набора данных (см. `tests/test_data/logs1/data`) в качестве входа для политики.
- Inference (инференс): в этом режиме политика выполняется в реальном времени (on-line). Используются реальные изображения с камеры и команды ввода с джойстика. Из этого режима можно переключиться в режим Joystick, нажав кнопку `A` на джойстике.
- Joystick (джойстик): в этом режиме робот управляется командами джойстика в одном из вариантов `control_mode`: «Dual» (управление левым и правым колесом левым и правым стиками) или «Joystick» (управление движением вперёд, назад, влево, вправо одним стиком). Сбор данных для обучения выполняется в режиме Joystick. Из этого режима можно переключиться в режим Inference, нажав кнопку `A` на джойстике.

Скрипт run.py принимает шесть аргументов (подробности см. в `run.py`):
```
--policy_path: Путь к файлу политики.
--dataset_path: Путь к набору данных. Используется только в режиме debug.
--log_path: Путь к папке журналов, куда сохраняются запуски.
--inference_backend: Используемый бэкенд. Для максимальной производительности рассмотрите экспорт всех моделей в формат openvino. Варианты: tf, tflite, openvino.
--mode: Режим запуска. Варианты: debug, inference, joystick.
--control_mode: Схема управления в режиме joystick. Варианты: dual, joystick.
```
## Генерация данных для обучения

Скрипт `generate_data_for_training.py` создаёт папку с журнальными данными, необходимую для обучения политики через скрипт `OpenBot/policy/openbot/train.py`. Папка с журнальными данными содержит подпапки `images` и `sensor_data` в формате, требуемом `train.py`.

Пример см. в `tests/test_generate_data.py`.

## OpenVino: оптимизация производительности инференса политики

Для оптимизации скорости инференса на поддерживаемом оборудовании Intel (например, на плате [Up Core Plus](https://up-board.org/upcoreplus/specifications/)) обученную модель необходимо экспортировать в OpenVino.

Скрипт `export_openvino.py` экспортирует обученную модель TensorFlow в модель OpenVino. Затем эта модель OpenVino загружается через `get_openvino_interpreter()` в `infer.py`.

Пример см. в `tests/test_export_openvino.py`.

## Тесты и пример кода

**Примечание:** для тестирования кода в папке `OpenBot/robot/linux/tests` должны находиться тестовые данные и тестовые модели `test_data` и `test_model` соответственно. Функция `get_data()` в `download_data.py` обеспечивает загрузку и вызывается в начале `test_infer.py`, `test_export_openvino.py` и `test_generate_data.py`. Альтернативно
запустите скрипт `get_test_data.sh` (только для unix-систем), который скачивает и распаковывает zip-архив, содержащий `test_data` и `test_models` — данные для режима debug и модели для инференса соответственно.

Запустите `pytest` в папке `tests` или выполните файлы `test_*.py` по отдельности, чтобы проверить функциональность

- загрузки тестовых данных и тестовой модели из облака — `test_download_data.py`
- экспорта в OpenVino — `test_export_openvino.py`
- генерации данных для обучения — `test_generate_data.py`.
- инференса в режиме debug для OpenVino, Tensorflow и Tflite — `test_infer.py`.
    - *Примечание*: тестовые данные в logs1 сгенерированы с помощью скрипта `associate_frames.py` в `OpenBot.policy.openbot`, при этом путь к изображениям жёстко прописан в `logs1/data/sensor_data/matched_frame_ctrl_cmd_processed.txt`.
    - `test_infer.py` заменяет его на путь к вашей копии репозитория `OpenBot`.
- подключения джойстика — `test_joystick.py`
- подключения моторов от последовательного порта к Arduino — `test_motor.py`.
- видеопотока к камере Realsense — `test_realsense.py`.

# Установка

Процесс установки подробно описан ниже.

Python-реализация управления OpenBot требует нескольких Python-модулей для инференса, управления джойстиком, восприятия и приведения в действие. Кроме того, могут потребоваться драйверы для камеры или контроллера.

## Настройка

В настоящее время код протестирован на:
- Плата: [Up Core Plus](https://up-board.org/upcoreplus/specifications/)
- Камера: [Realsense D435i](https://www.intelrealsense.com/depth-camera-d435i/)
- Контроллер: [Xbox One](https://www.microsoft.com/en-gb/store/collections/xboxcontrollers?source=lp)
- Arduino: [прошивка OpenBot](https://github.com/ob-f/OpenBot/blob/master/firmware/README.md)

## Python-модули

Код протестирован с Python 3.9. С использованием Anaconda3:
```
conda create --name openbot python==3.9
```

Сначала установите зависимости OpenBot.policy с помощью
```
cd ../../policy && pip install -r requirements.txt && cd ../robot/linux
```

Затем установите необходимые модули с помощью
```
pip install -r requirements.txt
```

В частности,
- `pyserial` обеспечивает связь с Arduino, а значит и с моторами, через последовательный порт
- `pyrealsense2` и `opencv-python` необходимы для обработки изображений с камеры.
- `pygame` используется для управления джойстиком и обработки вводов с джойстика
- `openvino-dev[tensorflow2,extras]` используется для повышения производительности на поддерживаемом оборудовании Intel. Подробнее об оптимизированном ИИ-инференсе на оборудовании Intel см. [OpenVino](https://docs.openvino.ai/latest/home.html). OpenVino — рекомендуемый бэкенд инференса. Также поддерживаются Tensorflow и Tflite (см. раздел «Тесты»). Для запуска модулей PyTorch рассмотрите конвертацию PyTorch в бэкенд OpenVino (см. [это руководство](https://docs.openvino.ai/latest/openvino_docs_MO_DG_prepare_model_convert_model_Convert_Model_From_PyTorch.html)).

## Драйверы

Если код запускается на Ubuntu, для USB-донгла беспроводного контроллера Xbox One требуется драйвер, который можно найти [по этой ссылке](https://github.com/medusalix/xone).

## Tensorflow для инференса

Если для инференса используется TensorFlow, добавьте Python-модуль `policy` в `PYTHONPATH` с помощью `export PYTHONPATH=$PYTHONPATH:/path/to/OpenBot/policy`. Этот обходной путь позволяет не устанавливать openbot как модуль и обеспечивает поиск `openbot.utils.load_model()`, необходимого для загрузки модели tensorflow. Подробности см. в `get_tf_interpreter()` в `infer.py` и в тестовом коде `tests/test_infer.py`.

## Поддержка нелюксовых дистрибутивов (MacOS, Windows)

Обратите внимание, что код предназначен для запуска на компьютерах под управлением Linux, например Up Core Plus. Некоторые Python-модули могут быть недоступны для MacOS или Windows.

Код может работать на MacOS в целях отладки при следующих изменениях:
- Используйте `pyrealsense2-macosx` вместо `pyrealsense2` в requirements.txt
- Для tflite следуйте [этим инструкциям](https://github.com/milinddeore/TfLite-Standalone-build-Linux-MacOS)
