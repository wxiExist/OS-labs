# OS-labs
![Static Badge](https://img.shields.io/badge/wxiExist-OS-labs) ![GitHub top language](https://img.shields.io/github/languages/top/wxiExist/OS-labs) ![GitHub issues](https://img.shields.io/github/issues/wxiExist/OS-labs) ![GitHub Repo stars](https://img.shields.io/github/stars/wxiExist/OS-labs)

![image](https://github.com/user-attachments/assets/0cea9b49-d978-48b7-b2ad-d7e9ccfff5de) 

# Программа для свёртки матрицы (конволюции) с многопоточностью

Данный проект демонстрирует, как выполнить **2D-свёртку** большой матрицы в несколько потоков на Windows.  
Код написан на языке C, используя **WinAPI** для создания и синхронизации потоков.

---

## Возможности

- **Генерация данных**  
  Создание входной матрицы и ядра (kernel) со случайными числами.
- **Многократная свёртка**  
  Параметр `K` указывает, сколько раз подряд применять фильтр.
- **Многопоточность**  
  Разделение строк матрицы между потоками до `maxThreads`.
- **Анализ производительности**  
  Можно менять размеры матрицы, ядра и число потоков, чтобы изучать ускорение.

---

## Параметры запуска

| Параметр       | Описание                                                    |
|----------------|------------------------------------------------------------|
| `<rows>`       | Количество строк в матрице.                                |
| `<cols>`       | Количество столбцов в матрице.                             |
| `<kernelSize>` | Размер свёрточного ядра (например, 3 для ядра 3×3).        |
| `<K>`          | Сколько раз подряд применять свёртку.                      |
| `<maxThreads>` | Максимальное число потоков для параллельных вычислений.    |

Пример команды:
```bash
main.exe <rows> <cols> <kernelSize> <K> <maxThreads>
main.exe 15 15 2 8 4


```
# Вывод

![image](https://github.com/user-attachments/assets/6f6da402-3707-4454-894f-f7c6c1df461b)

