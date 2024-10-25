//
// Created by Vladimir on 8.9.24..
//
#include "zip_operations.h"
#include <zip.h>
#include <filesystem>
#include <iostream>

void createZipArchive(const std::string& zipPath, const std::vector<std::string>& files) {
    int errorp;
    zip_t *archive = zip_open(zipPath.c_str(), ZIP_CREATE | ZIP_TRUNCATE, &errorp);
    if (!archive) {
        std::cerr << "Не удалось создать zip-архив: " << zipPath << " (Код ошибки: " << errorp << ")" << std::endl;
        return;
    }

    for (const auto& file : files) {
        zip_source_t *source = zip_source_file(archive, file.c_str(), 0, 0);
        if (!source) {
            std::cerr << "Не удалось создать zip источник для файла: " << file << std::endl;
            continue;
        }
        if (zip_file_add(archive, std::filesystem::path(file).filename().string().c_str(), source, ZIP_FL_OVERWRITE) < 0) {
            std::cerr << "Не удалось добавить файл в zip: " << file << " (Ошибка: " << zip_strerror(archive) << ")" << std::endl;
            zip_source_free(source);
            continue;
        }
    }

    if (zip_close(archive) < 0) {
        std::cerr << "Не удалось закрыть zip-архив: " << zipPath << " (Ошибка: " << zip_strerror(archive) << ")" << std::endl;
    }
}