#include "mode_simple.h"

#include <sys/types.h>
#include <sys/stat.h>

string init[] = { "Load a file", "Clear console screen", "Exit the application" };

// Initializes simple mode
void initModeSimple() {
	cin.sync();
	command = "";
	while (true) {
		printCommands(init, sizeof(init) / sizeof(*init));
		cout << "choise> ";
		cin >> command;

		if (command == "1") { // Load the file
			if (fileMenu() == -1) continue;
		} // Load the file
		
		else if (command == "2") { // Clear console
			system("cls");
		} // Clear console

		else if (command == "5") { // Exit
			break;
		} // Exit
		system("cls");
	} // Main while loop
}

string file_ops[] = { "Show information about the file", "Print contents", "Search file", "Edit row", "Delete row", "Insert row", "Save file", "Save file as...", "Export to HTML", "Exit file menu (close file)" };

// Opens the file menu
int fileMenu() {
	cout << "Enter file path and name (C:\\some_dir\\some_file.txt): ";
	string path;

// -------------------------------------------------------
	/* LOADING THE FILE */
	try {
		cin >> path;
		file.open(path);
		if (file.is_open()) { // file loaded
			parseFile(file, _table, _columns, _LINE_DELIMITER, _ELEMENT_DELIMITER);
			system("cls");
			cout << "File \"" << path << "\" loaded\n";
			_FILE_LOADED = true;

			struct stat filestatus;
			stat(path.c_str(), &filestatus);
			file_info.file_size = filestatus.st_size;
			file_info.file_path = path;
		}
		else { // file not loaded
			system("cls");
			cout << "Failed to open file \"" << path << "\"\n";
			return -1;
		} // file.is_open()
	}
	catch (exception) {
		system("cls");
		cout << "Failed to open file \"" << path << "\"\n";
		return -1;
	} // try-catch end
// -------------------------------------------------------

	/* Main UI part */
	bool b_printCommands = true;
	bool b_printContents = true;
	_current_display = _table;
	while (isJavaTheBest) { // File while loop
		if (b_printContents) {
			print(_current_display, _columns);
			cout << endl;
		}
		if (b_printCommands) {
			printCommands(file_ops, sizeof(file_ops) / sizeof(*file_ops));
		}
		cout << "choise> ";
		cin >> command;
		if (command == "1") {								// Show info
			system("cls"); b_printCommands = true; b_printContents = false;
			str_vec _info = info();
			for (int i = 0; i < _info.size(); i++) {
				cout << _info.at(i) << endl;
			}
			cout << endl;
		} // Info

		else if (command == "2") {							// Print contents
			system("cls"); b_printCommands = true; b_printContents = true;
			_current_display = _table;
			continue;
		}

		else if (command == "3") {							// Search file
			cout << "search> ";
			string search_str; cin >> search_str;
			matrix search_result = search(_table, search_str);
			system("cls"); b_printCommands = true; b_printContents = true;
			_current_display = search_result;
		}

		else if (command == "4") {							// Edit row
			if (!_current_display.empty()) {
				cout << "Select row (1 - " << _current_display.size() << "): ";
				int row_num; cin >> row_num;
				if (!cin.good() || row_num < 1 || row_num > _current_display.size()) {
					cout << "Bad row number!\n";
					b_printCommands = false; b_printContents = false;
					continue;
				}
				edit_row(_current_display.at(row_num - 1));
				system("cls"); b_printCommands = true; b_printContents = true;
				continue;
			}
		}

		else if (command == "5") {							// Delete row
			cout << "Select row (1 - " << _current_display.size() << "): ";
			int row_num; cin >> row_num;
			if (!cin.good() || row_num < 1 || row_num > _current_display.size()) {
				cout << "Bad row number!\n";
				b_printCommands = false;
				continue;
			}
		}

		else if (command == "6") {							// Insert row

		}

		else if (command == "7") {							// Save file

		}

		else if (command == "8") {							// Save file as

		}

		else if (command == "9") {							// Export to HTML

		}

		else if (command == "10") break;						// Exit file menu

		else {												// Wrong command
			system("cls"); b_printCommands = true; b_printContents = true;
			cout << "Wrong command \"" << command << "\"\n\n";
		}
	} // File while loop
}

string edit_row_ops[] = { "Edit element", "Delete element", "Save", "Cancel" };

int edit_row(str_vec& row) {
	while (isJavaTheBest) {
		system("cls");
		print_row(row);
		cout << endl;
		printCommands(edit_row_ops, sizeof(edit_row_ops) / sizeof(*edit_row_ops));
		cout << "choise> ";
		string command; cin >> command;
		if (command == "1") {
			cout << "Element number: ";
			int elem; cin >> elem;
			if (!cin.good() || elem < 1 || elem > row.size()) {
				cout << "Bad element number" << endl << endl;
				system("cls"); continue;
			}
			elem--;
			cout << "Old > " << row.at(elem) << endl;
			cout << "New > "; getline(cin, command, '\n'); getline(cin, command, '\n');
			row.at(elem).replace(row.at(elem).begin(), row.at(elem).end(), command);
			
		}

		else if (command == "2") {

		}

		else if (command == "3") {
			int orig_row_index = StrToInt(row.at(row.size() - 1)) - 1;
			_table.at(orig_row_index) = row;
			break;
		}

		else if (command == "4") {
			break;
		}
	}
	return -1;
}


void print_row(str_vec& row) {
	cout << "Row elements:" << endl;
	if (_columns.empty())
		for (int i = 0; i < row.size() - 1; i++) {
			cout << "[" << to_string(i + 1) << "] " << row.at(i) << endl;
		}
	else {
		for (int i = 0; i < _columns.size(); i++) {
			string tmp;
			try {
				cout << setw(4) << to_string(i + 1) + "" << setw(20) << "> [" + _columns.at(i) + "]" << row.at(i) << endl;
			}
			catch(exception) {
				cout << setw(4) << to_string(i + 1) + "" << setw(20) << "> [" + _columns.at(i) + "]" << endl;
			}
			
		}
	}
}

/**
 * Prints all strings from an array in this order: "index: element"
 * @param[in] arr	Array with commands
 * @param[in] size	Array size
 */
void printCommands(string arr[], int size) {
	for (int i = 0; i < size; i++) {
		cout << i + 1 << ": " << arr[i] << "\n";
	} // Prints availible commands
}