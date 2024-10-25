#include "image_processing.h"
#include "exif_operations.h"
#include "zip_operations.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <random>
#include <filesystem>

int main() {

    // Запрос у пользователя действия
    int choice;
    std::cout << "Выберите действие:\n";
    std::cout << "1 - Удалить метаданные\n";
    std::cout << "2 - Уникализировать изображение\n";
    std::cout << "3 - Уникализировать изображение и добавить случайные метаданные\n";
    std::cin >> choice;

    // Ввод пути к исходному изображению
    std::string inputFilename;
    std::cout << "Введите путь к изображению: ";
    std::cin.ignore();  // Очистить буфер после ввода числа
    std::getline(std::cin, inputFilename);

    // Удаление возможных лишних пробелов в начале и конце строки
    inputFilename.erase(inputFilename.find_last_not_of(" \n\r\t") + 1);
    inputFilename.erase(0, inputFilename.find_first_not_of(" \n\r\t"));

    std::cout << "Загружается изображение: " << inputFilename << std::endl;

    // Загрузка изображения
    cv::Mat originalImage = cv::imread(inputFilename);
    if (originalImage.empty()) {
        std::cerr << "Не удалось открыть или найти изображение: " << inputFilename << std::endl;
        return 1;
    }

    // Определение пути для сохранения zip-архива
    std::filesystem::path inputPath(inputFilename);
    std::string zipPath = inputPath.parent_path().string() + "/unique_images.zip";

    // Инициализация генератора случайных чисел
    std::random_device rd;
    std::mt19937 rng(rd());

    // Переменная для количества изображений
    int numImages = 0;

    // Обработка выбора пользователя
    if (choice == 1) {
        // Удаление метаданных
        removeExifData(inputFilename);
        std::cout << "Метаданные удалены из: " << inputFilename << std::endl;
    } else if (choice == 2 || choice == 3) {
        // Запрос количества изображений у пользователя
        std::cout << "Введите количество изображений для генерации: ";
        std::cin >> numImages;

        // Уникализация изображения и добавление метаданных при необходимости
        std::vector<std::string> outputFiles;
        for (int i = 1; i <= numImages; ++i) {
            cv::Mat uniqueImage = originalImage.clone();
            modifyImage(uniqueImage); // Функция уникализации изображения

            std::string outputFilename = inputPath.parent_path().string() + "/unique_image_" + std::to_string(i) + ".jpg";
            cv::imwrite(outputFilename, uniqueImage);

            if (choice == 3) {
                removeExifData(outputFilename); // Удаление исходных метаданных
                addRandomExifData(outputFilename, rng); // Добавление случайных EXIF-данных
            }

            outputFiles.push_back(outputFilename);
            std::cout << "Создано: " << outputFilename << std::endl;
        }

        // Создание zip-архива
        createZipArchive(zipPath, outputFiles);

        // Удаление файлов изображений после их добавления в архив
        for (const auto& file : outputFiles) {
            std::filesystem::remove(file);
            std::cout << "Удалено: " << file << std::endl;
        }

        std::cout << "Архив создан: " << zipPath << std::endl;
    } else {
        std::cerr << "Некорректный выбор действия." << std::endl;
        return 1;
    }

    return 0;
}