Написать консольное приложение, реализующее следующие методы:
1) Построение дерева каталога системы, начиная от заданного каталога. Т.е. на вход задаётся параметр, который определяет начиная от какого каталога строить дерево.
2) Параллельный поиск заданного файла по дереву.
Программа должна выполняться под Unix системой или Windows по желанию студента.

На вход программа принимает несколько параметров:
Обязательный параметр: 
Имя искомого файла. При поиске имя должно полностью соотвествовать искомому - случаи с частичным совпадением можно не рассматривать.
Дополнительные параметры:
--path - начальный каталог, от которого бдует строиться дерево. Является корневым узлом дерева. Если ключ не задан, строится от корня (для Unix систем - /, для Windows систем - C:/)
--num_threads - количество потоков, которое будет использоваться при поиске по дереву. Если ключ не задан, то использовать 10 потоков. Количество потоков, используемое при поиске, не должно превышать этого значения, но может быть меньше, если нет возможности распараллелить на большее количество, на данный момент работы программы.

Пример вызова для Unix системы:
./find_app some_file_name --path /home/user --num_threads 15

Требования к коду:
1) Единообразное соблюдение отступов.
При каждом использовании операторных скобок использовать символ табуляции.

Пример хорошего кода:
void searchFunction()
{
	bool flag = false;
	auto some_var = ...;
	if (some_var)
	{
		flag = true;
		return ;
	}	
}

Пример плохого кода:
void searchFunction()
{
bool flag = false;
auto some_var = ...;
if (some_var)
{
flag = true;
return ;
}
}

2) Понятное наименование функций и переменных.
Используйте имена функций и переменных, которые отражают смысл их работы. При именовании используйте единообразный стиль, т.е:

а)camelCase - начало имени с маленькой буквы, все последующие слова в имени с большой буквы:

size_t сounter = 0;
size_t elementsCount = 0;
void searchFunction()

б)snake_case - всё имя пишется с маленькой буквы, слова внутри имени разделяются подчёркиванием:

size_t сounter = 0;
size_t elements_count = 0;
void search_function()

Допускается либо использование только camelCase, либо только snake_case, либо именование ВСЕХ функций camelCase и ВСЕХ переменных snake_case и наоборот.

Пример плохого кода:
void f()
{
	int a = 42;
	jDn(a);
}




Сдача программы происходит путём отправки ссылки git репозитория на почту vgorbunov@bmstu.ru с темой письма "ОС АК3-51Б ДЗ ФИО". После проверки репозитория происходит личная защита на паре. При выявлении недочетов, программа отправляется на доработку.
В репозитории должно быть не менее двух коммитов с внятным комментарием описывающем степень выполнения программы на момент создания коммита. Один коммит должен быть создан после создания функции реализующей метод 1. Другой коммит должен быть создан по окончании реализации.


Дополнительная информация:
1) Получить доступ к аргументам, переданным программе, можно с помощью переменных argc\argv, в объявлении функцит main.
	argc - количество переданных параметров. Количество не меньше одного (первый параметр - само имя программы)
	argv - массив аргументов.
http://cppstudio.com/post/421/

2) Пример получения файлов в каталоге:
	а) средствами API ОС: http://iphone-sdk-programming.blogspot.com/2010/05/c-windows-unix-ms-dos.html
	б) Кроссплатформенный вариант от boost(требует предварительной настройки IDE и окружения): 
	https://www.boost.org/doc/libs/1_54_0/libs/filesystem/doc/reference.html
3) Начало работы с git:
	https://git-scm.com/book/ru/v2
	Для сдачи ДЗ достаточно освоить команды:
	git clone
	git add
	git commit -m
	git pull
	git push
