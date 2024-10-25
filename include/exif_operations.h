//
// Created by Vladimir on 8.9.24..
//

#ifndef EXIF_OPERATIONS_H
#define EXIF_OPERATIONS_H

#include <string>
#include <random>

// Функции для работы с EXIF-данными
void removeExifData(const std::string& filename);
void addRandomExifData(const std::string& filename, std::mt19937& rng);

// Вспомогательные функции для работы с GPS
void generateGPSCoordinates(std::mt19937& rng, double& latitude, double& longitude, std::string& latitudeRef, std::string& longitudeRef);
std::string createRationalCoordinate(double coord);

#endif // EXIF_OPERATIONS_H
