# Обработка большого файла

## Создатели:
* [Турушкин Егор Антонович](https://github.com/turueee)
* [Стрелков Кирилл Максимович](https://github.com/StrelkovKM)
* [Порфирьев Михаил Игоревич](https://github.com/Higraunb)

## Сборка и запуск: 

1. Клонируйте репозиторий и перейдите в ветку по tag: `V2.0.0`
   ```bash 
   git clone https://github.com/StrelkovKM/ProcessingLargeFiles.git
   cd ProcessingLargeFiles
   git chechout V2.0.0
   ```

   Для сборки проекта понадобиться `CMake` версии не ниже `3.10`
2. Из директории `ProcessingLargeFiles` откройте консоль и введите следующие команды:
   ```bash
   mkdir build && cd build
   cmake ..
   ```

3. В директории `src` в файле `main.cpp` в строку:
   ```cpp
   FileProcessing A("<path_file>"); 
   ```
   вставьте глобальный путь до считываемого файла вместо `<path_file>`

   Констурктор <u>самостоятельно запрашивает сколько свободной RAM есть у компьютера</u>, если нужно ее задать вручную использовать метод SetRAMSize, аргументом которого является объем памяти в байтах.
   Example:
   ```cpp
   A.setRAMSize(0.25 * 1024 * 1024 * 1024); // 0.25 GB
   ```


   Для `Visual Studio 2022`:
4. В папке build открыть `ProcessingLargeFiles.sln`
5. В проекте назначить исполняемый проект - `src`
6. Запустите программу (F5)

   Для `Visual Studio Code`:
4. Из директории `build` соберите зависимости и обновите `main.cpp`:
   ```bash
   make
   ```
5. Запустите исполняемый файл:
   ```bash
   ./src/src
   ```