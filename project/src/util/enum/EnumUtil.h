#pragma once
# include <map>
# include <string>
# include <stdarg.h>
# include <string.h>

//enumの定義と管理用mapの自動作成をおこなうマクロ
# define ENUM(_name, ...)								\
	struct _name {										\
		enum type {__VA_ARGS__, NUM};					\
		std::map <std::string, type> converter;			\
		_name() {										\
			createEnumMap(converter, #__VA_ARGS__, NUM, __VA_ARGS__);	\
		}												\
	}_name;

//列挙子を示す文字列をキーとし列挙定数の数値を値としたmapを作成する
template <class T> void createEnumMap(std::map<std::string, T>& _map, char* _list, int _num, ...) {
	char* listCopy = new char[255];
	char* tmpKey;
	strcpy(listCopy, _list);

	va_list args;
	va_start(args, _num);

	if ((tmpKey = strtok(listCopy, ", ")) != NULL) _map[tmpKey] = static_cast<T> (va_arg(args, int));
	for (int i = 1; i < _num; i++) {
		if ((tmpKey = strtok(NULL, ", ")) != NULL) _map[tmpKey] = static_cast<T> (va_arg(args, int));
	}
	va_end(args);
	delete[] listCopy;
}