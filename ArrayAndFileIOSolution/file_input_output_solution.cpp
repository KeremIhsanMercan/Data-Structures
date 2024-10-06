/* @Author
Student Name : Kerem Ýhsan Mercan
Student ID : 150210042
Date : 20.10.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

bool is_valid(int id_to_check, string file) {

    fstream my_file(file);
    string one_line, id;

    bool is_first_line_skipped = false;

    while (getline(my_file, one_line, '\n')) {

        if (!is_first_line_skipped)
        {
            is_first_line_skipped = true;
            continue;
        }

        stringstream one_line_split(one_line);
        getline(one_line_split, id, ';');

        if (stoi(id) == id_to_check)
        {
            return true;
        }
    }

    my_file.close();
    return false;
}


int main(int argc, char **argv) {

    // clock_t start = clock();

    if (argc != 3) {
        cout << argc << " file provided! It is not the right amount." << endl;
        return 0;
    }

    fstream hw1_file(argv[1], ios::in | ios::out);
    fstream operations_file(argv[2]);

    string one_line, last_emp_id, operation_name;
    int employee_count = 0;
    
    int last_emp_id_int;
    bool is_first_line_skipped = false;
    
    while (getline(hw1_file, one_line, '\n')) {

        if (!is_first_line_skipped)
        {
            is_first_line_skipped = true;
            continue;
        }

        stringstream one_line_split(one_line);
        getline(one_line_split, last_emp_id, ';');

        employee_count++;
    }

    last_emp_id_int = stoi(last_emp_id);

    while (getline(operations_file, one_line, '\n')) {

        stringstream one_line_split(one_line);
        getline(one_line_split, operation_name, ';');
        if (operation_name == "ADD")
        {

            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            hw1_file.seekp(ios::app);
            hw1_file << last_emp_id_int+1 << ";" << new_emp_salary << ";" << new_emp_department << endl;
            
            last_emp_id_int++;
            employee_count++;


        } else if (operation_name == "UPDATE")
        {
            string id_to_update, updated_salary, updated_department, temp_line, temp_id;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            if (!is_valid(stoi(id_to_update), argv[1]))
            {
                cout << "ERROR: An invalid ID to update" << endl;
                continue;
            }
            
            hw1_file.seekp(0);

            bool is_first_line_skipped = false;
            int line_to_update = 0;
            long line_position;

            while (getline(hw1_file, temp_line))
            {
                stringstream temp_line_split(temp_line);

                getline(temp_line_split, temp_id, ';');

                if (temp_id == id_to_update)
                {
                    line_position = hw1_file.tellp();
                    break;
                }
                
                line_to_update++;
            }
            
            hw1_file.seekp(line_position);

            hw1_file << id_to_update << ";" << updated_salary << ";" << updated_department << endl;


        } else if (operation_name == "DELETE")
        {
            string id_to_delete, temp_line, temp_id;
            getline(one_line_split, id_to_delete, '\n');

            if (employee_count <= 0)
            {
                cout << "ERROR: There is no Employee" << endl;
                continue;
            }
            
            if (!is_valid(stoi(id_to_delete), argv[1]))
            {
                cout << "ERROR: An invalid ID to delete" << endl;
                continue;
            }

            fstream hw1_file_delete(argv[1], ios::in | ios::out);
            ofstream temp_file("temp.csv");
            bool is_first_line_skipped = false;

            while (getline(hw1_file_delete, temp_line))
            {
                if (!is_first_line_skipped)
                {
                    temp_file << temp_line << endl;
                    is_first_line_skipped = true;
                    continue;
                }

                stringstream one_line_split(temp_line);
                getline(one_line_split, temp_id, ';');

                if (stoi(temp_id) == stoi(id_to_delete))
                {
                    continue;
                }
                
                temp_file << temp_line << endl;
            }
            
            hw1_file_delete.close();
            temp_file.close();
            remove(argv[1]);
            rename("temp.csv", argv[1]);

            employee_count--;
        }

    }

    operations_file.close();
    hw1_file.close();

    // clock_t end = clock();
    // cout << "execution time of all array solution program: " << (double)(end-start)*1000 / CLOCKS_PER_SEC << " miliseconds" << endl;


    return 0;
}
