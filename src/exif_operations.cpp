#include <exiv2/exiv2.hpp>
#include <sstream>
#include <random>
#include <iomanip>
#include <iostream>
#include <vector>

// Прототипы функций
void generateGPSCoordinates(std::mt19937& rng, double& latitude, double& longitude, double& altitude, std::string& latitudeRef, std::string& longitudeRef);
std::string createRationalCoordinate(double coord);
std::string generateTimestamp();

// Удаление EXIF-данных
void removeExifData(const std::string& filename) {
    try {
        std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
        if (!image) {
            throw std::runtime_error("Ошибка: невозможно открыть файл.");
        }
        image->readMetadata();
        image->clearMetadata();
        image->writeMetadata();
    } catch (const Exiv2::Error& e) {
        std::cerr << "Ошибка удаления EXIF-данных: " << e.what() << std::endl;
    }
}

// Генерация случайных данных EXIF
void addRandomExifData(const std::string& filename, std::mt19937& rng) {
    try {
        std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(filename);
        if (!image) {
            throw std::runtime_error("Ошибка: невозможно открыть файл.");
        }

        // Считывание существующих метаданных
        image->readMetadata();
        Exiv2::ExifData& exifData = image->exifData();

        // Генерация случайных значений EXIF
        std::uniform_real_distribution<double> apertureDist(1.4, 22.0);
        std::uniform_real_distribution<double> exposureDist(1.0 / 4000, 30.0);
        std::uniform_real_distribution<double> focalLengthDist(10.0, 200.0);
        std::uniform_int_distribution<int> isoDist(100, 3200);

        // LensModel
        int lensModelValue = isoDist(rng);
        exifData["Exif.Photo.LensModel"] = "LensModel" + std::to_string(lensModelValue);
        std::cout << "LensModel: LensModel" << lensModelValue << std::endl;

        // Aperture (FNumber) в формате дробного значения
        double apertureValue = apertureDist(rng);
        int apertureNumerator = static_cast<int>(apertureValue * 10);
        exifData["Exif.Photo.FNumber"] = Exiv2::Rational(apertureNumerator, 10);
        std::cout << "Aperture (FNumber): " << apertureNumerator / 10.0 << std::endl;

        // ExposureTime (выдержка) в формате 1/x
        double exposureValue = exposureDist(rng);
        int exposureDenominator = static_cast<int>(1.0 / exposureValue);
        if (exposureDenominator == 0) exposureDenominator = 1;
        exifData["Exif.Photo.ExposureTime"] = Exiv2::Rational(1, exposureDenominator);
        std::cout << "ExposureTime: 1/" << exposureDenominator << " seconds" << std::endl;

        // FocalLength (фокусное расстояние) как дробное значение
        double focalLengthValue = focalLengthDist(rng);
        int focalLengthNumerator = static_cast<int>(focalLengthValue * 10);
        exifData["Exif.Photo.FocalLength"] = Exiv2::Rational(focalLengthNumerator, 10);
        std::cout << "FocalLength: " << focalLengthNumerator / 10.0 << " mm" << std::endl;


        // Flash (случайное значение из допустимых опций)
        std::vector<int> flashOptions = {0x0000, 0x0001, 0x0005, 0x0007, 0x0009, 0x0010};
        std::uniform_int_distribution<int> flashOptionDist(0, flashOptions.size() - 1);
        int flashValue = flashOptions[flashOptionDist(rng)];
        exifData["Exif.Photo.Flash"] = static_cast<uint16_t>(flashValue); // Обеспечим точный тип
        std::cout << "Flash: 0x" << std::hex << flashValue << std::dec << std::endl;

        // ISO
        int isoValue = isoDist(rng);
        exifData["Exif.Photo.ISOSpeedRatings"] = static_cast<uint16_t>(isoValue);
        std::cout << "ISO: " << isoValue << std::endl;

        // Генерация и запись GPS-координат
        double latitude = 0, longitude = 0, altitude = 0;
        std::string latitudeRef = "N", longitudeRef = "E";
        generateGPSCoordinates(rng, latitude, longitude, altitude, latitudeRef, longitudeRef);

        std::cout << "GPS Coordinates:" << std::endl;
        std::cout << "  Latitude: " << latitude << " " << latitudeRef << std::endl;
        std::cout << "  Longitude: " << longitude << " " << longitudeRef << std::endl;
        std::cout << "  Altitude: " << altitude << " meters" << std::endl;

        // GPS Latitude и Longitude как дроби
        std::unique_ptr<Exiv2::Value> latValue = Exiv2::Value::create(Exiv2::unsignedRational);
        latValue->read(createRationalCoordinate(latitude));
        exifData.add(Exiv2::ExifKey("Exif.GPSInfo.GPSLatitude"), latValue.get());

        std::unique_ptr<Exiv2::Value> lonValue = Exiv2::Value::create(Exiv2::unsignedRational);
        lonValue->read(createRationalCoordinate(longitude));
        exifData.add(Exiv2::ExifKey("Exif.GPSInfo.GPSLongitude"), lonValue.get());

        // GPS Latitude/Longitude Ref
        exifData["Exif.GPSInfo.GPSLatitudeRef"] = latitudeRef;
        exifData["Exif.GPSInfo.GPSLongitudeRef"] = longitudeRef;

        // GPS Altitude
        exifData["Exif.GPSInfo.GPSAltitude"] = Exiv2::Rational(static_cast<int>(altitude * 100), 100);
        exifData["Exif.GPSInfo.GPSAltitudeRef"] = altitude >= 0 ? 0 : 1;

        // Время UTC
        std::string timestamp = generateTimestamp();
        exifData["Exif.GPSInfo.GPSTimeStamp"] = timestamp;
        std::cout << "Timestamp (UTC): " << timestamp << std::endl;

        // Направление изображения (дробь)
        exifData["Exif.GPSInfo.GPSImgDirection"] = Exiv2::Rational(65231, 1000);
        std::cout << "Image Direction: " << 65231 / 1000.0 << " degrees" << std::endl;

        // Скорость
        exifData["Exif.GPSInfo.GPSSpeed"] = Exiv2::Rational(0, 1);
        exifData["Exif.GPSInfo.GPSSpeedRef"] = 'K';
        std::cout << "Speed: 0 km/h" << std::endl;

        // Запись метаданных обратно в изображение
        image->writeMetadata();
        std::cout << "EXIF-данные успешно записаны в файл " << filename << std::endl;
    } catch (const Exiv2::Error& e) {
        std::cerr << "Ошибка добавления EXIF-данных: " << e.what() << std::endl;
    }
}
// Реализация функции generateGPSCoordinates
void generateGPSCoordinates(std::mt19937& rng, double& latitude, double& longitude, double& altitude, std::string& latitudeRef, std::string& longitudeRef) {
    std::uniform_real_distribution<double> latitudeDist(-60.0, 70.0); // Широта от -60 до 70
    std::uniform_real_distribution<double> longitudeDist(-170.0, 170.0); // Долгота от -170 до 170
    std::uniform_real_distribution<double> altitudeDist(-100.0, 5000.0); // Высота от -100 до 5000 метров

    latitude = latitudeDist(rng);
    longitude = longitudeDist(rng);
    altitude = altitudeDist(rng);

    latitudeRef = (latitude >= 0) ? "N" : "S";
    longitudeRef = (longitude >= 0) ? "E" : "W";
}
// Преобразование GPS-координат в формат дробей
std::string createRationalCoordinate(double coord) {
    coord = std::abs(coord);
    int degrees = static_cast<int>(coord);
    double fractional = (coord - degrees) * 60;
    int minutes = static_cast<int>(fractional);
    double seconds = (fractional - minutes) * 60;

    std::ostringstream coordinate;
    coordinate << degrees << "/1 " << minutes << "/1 " << static_cast<int>(seconds * 10000) << "/10000";
    return coordinate.str();
}

// Генерация случайного времени UTC
std::string generateTimestamp() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> hourDist(0, 23);
    std::uniform_int_distribution<int> minuteDist(0, 59);
    std::uniform_int_distribution<int> secondDist(0, 59);

    int hour = hourDist(rng);
    int minute = minuteDist(rng);
    int second = secondDist(rng);

    std::ostringstream timestamp;
    timestamp << hour << "/1 " << minute << "/1 " << second << "/1";
    return timestamp.str();
}
