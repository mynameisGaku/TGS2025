#pragma once
# include <map>
# include <string>
# include <stdarg.h>
# include <string.h>

//enum�̒�`�ƊǗ��pmap�̎����쐬�������Ȃ��}�N��
# define ENUM(_name, ...)								\
	struct _name {										\
		enum type {__VA_ARGS__, NUM};					\
		std::map <std::string, type> converter;			\
		_name() {										\
			createEnumMap(converter, #__VA_ARGS__, NUM, __VA_ARGS__);	\
		}												\
	}_name;

//�񋓎q��������������L�[�Ƃ��񋓒萔�̐��l��l�Ƃ���map���쐬����
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