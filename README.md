# 2GIS_test_assign
Тестовое задание 2GIS на позицию Android C++/Qt/QML разработчик.

* Модуль Executor стартует в отдельном потоке функцию, читающую файл.

* Модуль FileWorker разбирает файл и строит мультииндекс структуру. Для построения топа был выбран модуль multi_index библиотеки Boost, позволяющий упорядочивать элементы по хэш-значениям слов и кол-ву вхождений слова.

* Модуль WordCount, собственно, описывает хелпер и multi_index структуры.

Общение UI и бекенда построено на MOC системе Qt с помощью механизма сигнал-слотов. Не знаю насколько корректно реализовано взаимодействие именно в QML файле, но я ориентировался на doc.qt и примеры оттуда. По сути первый проект >> hello_world на QML.

Алгоритм построения топа в цикле считывает слова, (меняет ключ/создаёт новую запись) в зависимости от условия, сравнивает новый топ с предыдущим и, при расхождении, посылает сигнал для перерисовки чарта.

Для реализации гистограммы, вопреки заданию, был выбран модуль ChartView. Я топлю за его удобство и простоту :) Мне показалось не очень удобным городить вермишель из 15 объектов или репитора и динамически считать позиции, высоту и прочие параметры для отрисовки.

Задания под звёздочкой, к сожалению, не успел реализовать. Предполагалось использовать QMutex и QWaitCondition с Boolean флагом для отслежования состояния работы потока.

# UPDATE! 
В ветке draft лежат сорсы с реализованными кнопками 'Пауза' и 'Отмена', правда, увы, с опозданием :(
