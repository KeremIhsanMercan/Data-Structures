/* @Author
StudentName : Kerem Ýhsan Mercan
StudentID : 150210042
Date :31.12.2023 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
using namespace std;

class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d)
        {
            id = i;
            salary = s;
            department = d;
        }
        void set_id(int new_id)
        {
            id = new_id;
        }
        void set_salary(int new_salary)
        {
            salary = new_salary;
        }
        void set_department(int new_department)
        {
            department = new_department;
        }
        int get_id()
        {
            return id;
        }
        int get_salary()
        {
            return salary;
        }
        int get_department()
        {
            return department;
        }
};

class QuadruplySkipList_Node {
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;
    public:
        QuadruplySkipList_Node(Employee *data)
        {
            this->data = data;
            this->next = NULL;
            this->prev = NULL;
            this->below = NULL;
            this->above = NULL;
        }
        void set_emp(Employee* data)
        {
            this->data = data;
        }
        void set_next(QuadruplySkipList_Node* input)
        {
            this->next = input;
        }
        void set_prev(QuadruplySkipList_Node* input)
        {
            this->prev = input;
        }
        void set_below(QuadruplySkipList_Node* input)
        {
            this->below = input;
        }
        void set_above(QuadruplySkipList_Node* input)
        {
            this->above = input;
        }
        Employee* get_emp()
        {
            return this->data;
        }
        QuadruplySkipList_Node* get_next()
        {
            return this->next;
        }
        QuadruplySkipList_Node* get_prev()
        {
            return this->prev;
        }
        QuadruplySkipList_Node* get_below()
        {
            return this->below;
        }
        QuadruplySkipList_Node* get_above()
        {
            return this->above;
        }
        int get_next_id()
        {
            if (this->get_next() == NULL)
            {
                return INT32_MAX;
            }
            
            return this->get_next()->get_emp()->get_id();
        }
};

class QuadruplySkipList {
    private:
        QuadruplySkipList_Node* head;
        int height;
    public:
        QuadruplySkipList(int height_in)
        {
            height = height_in;

            Employee* head_emp = new Employee(-1, 0, 0);
            head = new QuadruplySkipList_Node(head_emp);
            head->set_next(NULL);
            head->set_prev(NULL);
            head->set_below(NULL);
            head->set_above(NULL);

            QuadruplySkipList_Node* iter = head;
            for (int i = 0; i < height_in-1; i++)
            {
                iter->set_below(new QuadruplySkipList_Node(head_emp));
                iter->get_below()->set_above(iter);
                iter = iter->get_below();
            }
            
        }
        ~QuadruplySkipList()
        {
            QuadruplySkipList_Node* current = head;
            while (current->get_below() != NULL)
            {
                current = current->get_below();
            }
            
            while (current != NULL)
            {
                QuadruplySkipList_Node* current_p = current;
                current = current->get_next();

                delete current_p->get_emp();

                while (current_p != NULL)
                {
                    QuadruplySkipList_Node* current_p_below = current_p;
                    current_p = current_p->get_above();
                    delete current_p_below;
                }
                
            }
            
        }
        void insert(Employee* data)
        {
            int node_height_count = 0;

            while (node_height_count < (height-1) && (rand() % 2) == 1)
            {
                node_height_count++;
            }

            QuadruplySkipList_Node* current = head;
            QuadruplySkipList_Node* current_p;
            while (current != NULL)
            {
                current_p = current;
                if (current->get_next_id() > data->get_id())
                {
                    current = current->get_below();
                } else
                {
                    current = current->get_next();
                }
            }

            current = current_p;

            QuadruplySkipList_Node* base = new QuadruplySkipList_Node(data);
            base->set_next(current->get_next());
            
            if (base->get_next() != NULL)
            {
                base->get_next()->set_prev(base);
            }
            
            base->set_prev(current);
            current->set_next(base);
            QuadruplySkipList_Node* iter = base;

            for (int i = 0; i < node_height_count; i++)
            {
                iter->set_above(new QuadruplySkipList_Node(data));
                iter->get_above()->set_below(iter);

                QuadruplySkipList_Node* prev_above_iter = iter->get_prev()->get_above();
                QuadruplySkipList_Node* prev_above_iter_below = iter->get_prev();

                while (prev_above_iter == NULL)
                {
                    prev_above_iter = prev_above_iter_below->get_prev()->get_above();
                    prev_above_iter_below = prev_above_iter_below->get_prev();
                }
                
                iter->get_above()->set_next(prev_above_iter->get_next());

                iter->get_above()->set_prev(prev_above_iter);
                prev_above_iter->set_next(iter->get_above());

                if (iter->get_above()->get_next() != NULL)
                {
                    iter->get_above()->get_next()->set_prev(iter->get_above());
                }
                
                
                iter = iter->get_above();
            }
        }
        void remove(int remove_id)
        {
            QuadruplySkipList_Node* bottom_left = head;

            while (bottom_left->get_below() != NULL)
            {
                bottom_left = bottom_left->get_below();
            }
            
            if (bottom_left->get_next() == NULL)
            {
                cout << "ERROR: There is no Employee" << "\n";
                return;
            }

            QuadruplySkipList_Node* current = head;
            while (current->get_emp()->get_id() != remove_id)
            {
                
                if (current->get_next_id() > remove_id)
                {
                    current = current->get_below();
                } else
                {
                    current = current->get_next();
                }

                if (current == NULL)
                {
                    cout << "ERROR: An invalid ID to delete" << "\n";
                    return;
                }
            }

            delete current->get_emp();
            while (current != NULL)
            {
                QuadruplySkipList_Node* current_abv = current;
                current = current->get_below();
                current_abv->get_prev()->set_next(current_abv->get_next());

                if (current_abv->get_next() != NULL)
                {
                    current_abv->get_next()->set_prev(current_abv->get_prev());
                }

                delete current_abv;
            }            
        }
        Employee* search(int search_id)
        {
            QuadruplySkipList_Node* current = head;
            while (current->get_emp()->get_id() != search_id)
            {
                
                if (current->get_next_id() > search_id)
                {
                    current = current->get_below();
                } else
                {
                    current = current->get_next();
                }

                if (current == NULL)
                {
                    return NULL;
                }
            }

            return current->get_emp();
        }
        void update_emp(int id_to_update, int new_salary, int new_department)
        {

            Employee* emp_to_update = search(id_to_update);

            if (emp_to_update == NULL)
            {
                cout << "ERROR: An invalid ID to update" << "\n";
                return;
            }
            
            emp_to_update->set_salary(new_salary);
            emp_to_update->set_department(new_department);

        }
        void dumpToFile(ofstream& out_file)
        {
            QuadruplySkipList_Node* iter = head;

            while (iter->get_below() != NULL)
            {
                iter = iter->get_below();
            }

            iter = iter->get_next(); // to skip head node (-1)

            while (iter != NULL)
            {
                Employee* iter_emp = iter->get_emp();
                out_file << iter_emp->get_id() << ";" << iter_emp->get_salary() << ";" << iter_emp->get_department() << "\n";
                iter = iter->get_next();
            }
        }
};

int main(int argc, char** argv) {
    
    // clock_t start = clock();

    if (argc != 3)
    {
        cout << argc << " file provided! It is not the right amount." << "\n";
        return 0;
    }

    srand(time(0));

    QuadruplySkipList my_skip_list(20);
    
    fstream data_set(argv[1]);

    string one_line;
    string id_temp, salary_temp, department_temp;
    string first_line;
    bool is_first_line_skipped = false;
    int highest_id;

    while (getline(data_set, one_line, '\n'))
    {
        if (!is_first_line_skipped)
        {
            first_line = one_line;
            is_first_line_skipped = true;
            continue;
        }
        
        stringstream one_line_split(one_line);

        getline(one_line_split, id_temp, ';');
        getline(one_line_split, salary_temp, ';');
        getline(one_line_split, department_temp, '\n');

        Employee* new_employee = new Employee(stoi(id_temp), stoi(salary_temp), stoi(department_temp));

        my_skip_list.insert(new_employee);

        highest_id = max(highest_id, stoi(id_temp));

    }
    data_set.close();

    fstream operations_file_var(argv[2]);
    string operation_name;

    while (getline(operations_file_var, one_line, '\n'))
    {

        stringstream one_line_split(one_line);
        getline(one_line_split, operation_name, ';');        

        if (operation_name == "ADD")
        {
            string new_emp_salary, new_emp_department;
            getline(one_line_split, new_emp_salary, ';');
            getline(one_line_split, new_emp_department, '\n');

            highest_id++;

            Employee* new_employee = new Employee(highest_id, stoi(new_emp_salary), stoi(new_emp_department));
        
            my_skip_list.insert(new_employee);

        } else if (operation_name == "UPDATE")
        {
            string id_to_update, updated_salary, updated_department;
            getline(one_line_split, id_to_update, ';');
            getline(one_line_split, updated_salary, ';');
            getline(one_line_split, updated_department, '\n');

            my_skip_list.update_emp(stoi(id_to_update), stoi(updated_salary), stoi(updated_department));

        } else if (operation_name == "DELETE")
        {
            string id_to_delete;
            getline(one_line_split, id_to_delete, '\n');

            my_skip_list.remove(stoi(id_to_delete));

        }
    }
    operations_file_var.close();

    ofstream output_file_var("quadruply_output.csv");
    output_file_var << first_line << "\n";
    my_skip_list.dumpToFile(output_file_var);

    output_file_var.close();

    // clock_t end = clock();
    // cout << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "\n";

    return 0;
}