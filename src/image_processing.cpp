//
// Created by Vladimir on 8.9.24..
//
#include "image_processing.h"

// Функция для добавления случайного шума к изображению
void addNoise(cv::Mat& image) {
    cv::Mat noise = cv::Mat(image.size(), image.type());
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(1)); // Среднее 0, стандартное отклонение 1
    image += noise;
}

// Функция для изменения пикселей изображения
void modifyImage(cv::Mat& image) {
    addNoise(image);
}
