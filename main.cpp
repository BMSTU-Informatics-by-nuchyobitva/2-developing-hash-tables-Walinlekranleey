#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

class HashTable {
private:
    struct GeoPoint {
        //точка на карте
        double latitude;
        double longitude;
        std::string info;
        bool occupied;
        bool deleted;
        GeoPoint() : latitude(0), longitude(0), info(""), occupied(false), deleted(false) {}
        GeoPoint(double lat, double lon, const std::string& inf, bool occ, bool del)
        : latitude(lat), longitude(lon), info(inf), occupied(occ), deleted(del) {}
    };

    std::vector<GeoPoint> table;
    size_t capacity;
    size_t size;

    size_t Hash(double latitude, double longitude) {
        //хэш-функция
        size_t hash1 = static_cast<size_t>(latitude * 1000000) ^ (static_cast<size_t>(latitude * 1000000) >> 3);
        size_t hash2 = static_cast<size_t>(longitude * 1000000) ^ (static_cast<size_t>(longitude * 1000000) >> 5);
        return (hash1 * 2654435761 + hash2) % capacity;
    }

    void resize() {
        size_t newCapacity = capacity * 2;
        std::vector<GeoPoint> newTable(newCapacity);

        for (const auto& entry : table) {
            if (entry.occupied && !entry.deleted) {
                size_t index = Hash(entry.latitude, entry.longitude) % newCapacity;
                while (newTable[index].occupied) {
                    index = (index + 1) % newCapacity;
                }
                newTable[index] = entry;
            }
        }

        table = std::move(newTable);
        capacity = newCapacity;
    }

public:
    HashTable(size_t cap) : capacity(cap), size(0) {
        table.resize(capacity);
    }

    bool insert(double latitude, double longitude, const std::string& info) {
        if (size >= capacity * 0.7) {
            resize(); //если таблица переполнена
        }

        size_t index = Hash(latitude, longitude);
        size_t start_index = index;

        while (table[index].occupied && !table[index].deleted) {
            if (table[index].latitude == latitude && table[index].longitude == longitude) {
                table[index].info = info;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == start_index) return false;
        }

        table[index] = {latitude, longitude, info, true, false};
        size++;
        return true;
    }

    bool search(double latitude, double longitude, std::string &info) {
        size_t index = Hash(latitude, longitude);
        size_t start_index = index;

        while (table[index].occupied) {
            if (table[index].latitude == latitude && table[index].longitude == longitude && !table[index].deleted) {
                info = table[index].info;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == start_index) break;
        }
        return false;
    }

    bool remove(double latitude, double longitude) {
        size_t index = Hash(latitude, longitude);
        size_t start_index = index;

        while (table[index].occupied) {
            if (table[index].latitude == latitude && table[index].longitude == longitude && !table[index].deleted) {
                table[index].deleted = true;
                size--;
                return true;
            }
            index = (index + 1) % capacity;
            if (index == start_index) break;
        }
        return false;
    }

    void display() {
        std::cout << std::fixed << std::setprecision(6);
        for (size_t i = 0; i < capacity; i++) {
            if (table[i].occupied && !table[i].deleted)
                std::cout << i << ": (" << table[i].latitude << ", " << table[i].longitude << ") -> " << table[i].info << "\n";
            else
                std::cout << i << ": --\n";
        }
    }
};

void menu() {
    std::cout << "\nМеню:\n";
    std::cout << "1. Добавить координаты\n";
    std::cout << "2. Найти координаты\n";
    std::cout << "3. Удалить координаты\n";
    std::cout << "4. Показать таблицу\n";
    std::cout << "5. Выход\n";
    std::cout << "Выберите действие: ";
}


int main() {
    HashTable ht(10);
    int choice;
    while (true) {
        menu();
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (choice == 5) break;

        double latitude, longitude;
        std::string info;
        switch (choice) {
            case 1:
                std::cout << "Введите широту: ";
            std::cin >> latitude;
            std::cout << "Введите долготу: ";
            std::cin >> longitude;
            std::cin.ignore();
            std::cout << "Описание: ";
            std::getline(std::cin, info);
            ht.insert(latitude, longitude, info);
            break;
            case 2:
                std::cout << "Введите широту: ";
            std::cin >> latitude;
            std::cout << "Введите долготу: ";
            std::cin >> longitude;
            if (ht.search(latitude, longitude, info))
                std::cout << "Найдено: " << info << "\n";
            else
                std::cout << "Не найдено\n";
            break;
            case 3:
                std::cout << "Введите широту: ";
            std::cin >> latitude;
            std::cout << "Введите долготу: ";
            std::cin >> longitude;
            if (ht.remove(latitude, longitude))
                std::cout << "Удалено\n";
            else
                std::cout << "Не найдено\n";
            break;
            case 4:
                ht.display();
            break;
            default:
                std::cout << "Что-то пошло не так.\n";
        }
    }
}

