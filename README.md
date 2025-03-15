**Отчет о хэш-таблице с географическими координатами**

### 1. Описание структуры данных
Разработанная хэш-таблица предназначена для хранения объектов типа `GeoPoint`, которые представляют собой географические точки с дополнительной информацией. Каждый объект содержит:
- `latitude` (широта) — координата точки.
- `longitude` (долгота) — координата точки.
- `info` (строка) — описание местоположения.
- `occupied` (булево) — флаг занятости ячейки.
- `deleted` (булево) — флаг удаления элемента.

### 2. Метод хэширования
Для вычисления индекса ячейки используется комбинированный хэш-функция:
1. Преобразование координат (`latitude` и `longitude`) в целые числа с точностью до 6 знаков после запятой.
2. Применение битовых операций (`^` и `>>`) для рандомизации значений.
3. Использование константы `2654435761` (множитель из умной книжки Кнута про алгоритмы, другое название - соль) для уменьшения коллизий.
4. Нахождение остатка от деления на размер таблицы.

### 3. Методы обработки коллизий
В качестве метода разрешения коллизий используется **открытая адресация с линейным пробингом**:
- Если хэш-ячейка занята, поиск продолжается в следующей по модулю `capacity` ячейке (`index = (index + 1) % capacity`).
- Если найден существующий ключ, обновляется информация (`info`).
- Если найдена пустая или удаленная ячейка, вставляется новый элемент.

### 4. Автоматическое расширение таблицы
При заполнении более 70% от общего объема вызывается метод `resize()`, который:
1. Увеличивает размер таблицы в 2 раза.
2. Создает новый массив элементов.
3. Перехеширует все существующие элементы в новый массив.

### 5. Основные методы
- `insert(latitude, longitude, info)` — добавление нового элемента или обновление существующего.
- `search(latitude, longitude, info)` — поиск информации по координатам.
- `remove(latitude, longitude)` — удаление элемента (помечается как `deleted`).
- `display()` — вывод содержимого таблицы в консоль.

### 6. Пример работы
```cpp
HashTable ht(5);
ht.insert(40.7128, -74.0060, "New York City");
ht.insert(34.0522, -118.2440, "Los Angeles");
ht.insert(51.5074, -0.1278, "London");
ht.insert(48.8566, 2.3522, "Paris");
ht.insert(35.6895, 139.6917, "Tokyo");
ht.insert(55.7558, 37.6173, "Moscow");
ht.display();
```

### 7. Интерактивное взаимодействие
В качестве тестирования хэш-таблицы было реализовано интерактивное взаимодействие с ней по средствам "Меню"
```
1. Добавить координаты
2. Найти координаты
3. Удалить координаты
4. Показать таблицу
5. Выход
```
