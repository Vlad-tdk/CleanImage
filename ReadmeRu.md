# CleanImage

**CleanImage** — это кроссплатформенное приложение для очистки изображений и добавления случайных EXIF-данных, созданное с использованием C++, OpenCV, Exiv2 и libzip. Проект поддерживает компиляцию на Windows, Linux и macOS.

## Функции
- Уникализатор изображений
- Удаление EXIF-метаданных из изображений
- Генерация случайных EXIF-данных (включая GPS-координаты, время съемки, настройки камеры и т.д.)
- Архивация результатов в ZIP-файл

## Зависимости

Для успешной сборки проекта требуется:

- CMake 3.14 или новее
- C++20 компилятор
- OpenCV для обработки изображений
- libzip для работы с ZIP-файлами
- Exiv2 для работы с EXIF-данными
- INIReader для чтения конфигураций (библиотека inih)

### Установка зависимостей

#### Windows

1. Установите `vcpkg` и добавьте его в PATH.
2. Установите необходимые пакеты:

```bash
vcpkg install opencv libzip exiv2
```
#### Linux
1. Установите пакеты через пакетный менеджер (например, apt для Ubuntu):
```bash
sudo apt update
sudo apt install libopencv-dev libzip-dev libexiv2-dev cmake g++
```
#### MacBook
1. Установите Homebrew для управления пакетами.
2. Установите пакеты:
```bash
brew install opencv libzip exiv2 cmake
```
### Сборка проекта
1. Клонируйте репозиторий:
```bash
git clone https://github.com/your_profile/CleanImage.git
cd CleanImage
```
2. Настройте проект с помощью CMake:
```bash
mkdir build
cd build
cmake ..
```
3. Соберите проект:
```bash
cmake --build .
```
##### Примечание: В macOS укажите путь к OpenCV в CMakeLists.txt через set(OpenCV_DIR "path_to_vcpkg/installed/arm64-osx/share/opencv4").
4. Использование
   После успешной сборки, запускайте CleanImage для выполнения действий:
```bash
 ./CleanImage
```
### Пример использования
Запустите программу и выберите действие (удаление метаданных, добавление случайных EXIF-данных и т.д.).
Введите путь к изображению, с которым хотите работать.
Выберите количество изображений и подтвердите.
Результаты будут сохранены в ZIP-архиве в указанной директории.

### Структура проекта
- src/ — исходный код:
- main.cpp — основной файл для запуска программы.
- image_processing.cpp — функции для обработки изображений.
- exif_operations.cpp — функции для работы с EXIF-данными.
- zip_operations.cpp — функции для создания ZIP-архива.
- include/ — заголовочные файлы.
- CMakeLists.txt — файл для настройки сборки.
## Поддержка
Если у вас возникли проблемы или есть предложения, откройте задачу (issue) в репозитории или отправьте pull request.