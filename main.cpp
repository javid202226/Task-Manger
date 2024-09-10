#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
using namespace std;

class Date
{
public:
    int day;
    int month;
    int year;

public:
    Date()
    {
        day = 0;
        month = 0;
        year = 0;
    }

    Date(int day, int month, int year)
    {
        if (!isValidDate(day, month, year))
        {
            throw std::invalid_argument("Invalid date values.");
        }

        this->day = day;
        this->month = month;
        this->year = year;
    }

    static bool isValidDate(int day, int month, int year)
    {
        if (year < 0 || month < 1 || month > 12)
        {
            return false;
        }

        if (day < 1 || day > 31)
        {
            return false;
        }

        return true;
    }

    void print_date()
    {
        cout << day << "/" << month << "/" << year;
    }

    bool operator==(Date &other)
    {
        return day == other.day && month == other.month && year == other.year;
    }

    bool operator!=(Date &other)
    {
        return !(*this == other);
    }

    bool operator<(Date &other)
    {
        if (year != other.year)
        {
            return year < other.year;
        }
        if (month != other.month)
        {
            return month < other.month;
        }
        return day < other.day;
    }

    bool operator<=(Date &other)
    {
        return (*this < other) || (*this == other);
    }

    bool operator>(Date &other)
    {
        return !(*this <= other);
    }

    bool operator>=(Date &other)
    {
        return !(*this < other);
    }
};

class task
{
private:
    int priority;
    string task_name;
    Date deadline;

public:
    int task_id;
    task(int p, int tid, string tname, Date d)
    {
        priority = p;
        task_id = tid;
        task_name = tname;
        deadline = d;
    }

    Date current_date()
    {
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);

        tm *timeStruct = localtime(&currentTime);

        int y = timeStruct->tm_year + 1900;
        int m = timeStruct->tm_mon + 1;
        int d = timeStruct->tm_mday;
        return Date(d, m, y);
    }

    void print_task()
    {

        Date result;
        cout << "Task ID: " << task_id << endl;
        cout << "Task Name: " << task_name << endl;
        cout << "Priority: " << priority << endl;
        cout << "Deadline: ";
        deadline.print_date();
        Date curr = current_date();
        cout << endl;
    }

    Date &getDeadline()
    {
        return deadline;
    }
    int getPriority()
    {
        return priority;
    }
};

bool compareTasksByDeadline(task task1, task task2)
{
    return task1.getDeadline() < task2.getDeadline();
}

bool compareTasksByPriority(task task1, task task2)
{
    return task1.getPriority() < task2.getPriority();
}

class user
{
    string name;
    vector<task> tasks;

public:
    user(string n)
    {
        name = n;
    }

    bool addTask(task t)
    {
        for (auto i : tasks)
        {
            if (t.task_id == i.task_id)
            {
                cout << "Task with same id already exists" << endl;
                return 0;
            }
        }
        tasks.push_back(t);
        sort(tasks.begin(), tasks.end(), compareTasksByDeadline);
        sort(tasks.begin(), tasks.end(), compareTasksByPriority);
        return 1;
    }

    bool removeTask(int tid)
    {
        for (auto it = tasks.begin(); it != tasks.end(); ++it)
        {
            if ((*it).task_id == tid)
            {
                tasks.erase(it);
                cout << "Task.deleted" << endl;
                return 1;
            }
        }
        return 0;
    }

    bool empty()
    {
        if (tasks.size() == 0)
            return 1;
        return 0;
    }

    void printTasks()
    {
        for (auto task : tasks)
        {
            cout << "---------------------------" << endl;
            task.print_task();
            cout << "---------------------------" << endl;
        }
    }
};

int main()
{
    try
    {
        string name;
        cout << "Enter User Name: ";
        getline(cin, name);
        user user(name);

        while (true)
        {
            cout << endl;

            cout << "1. Add Task" << endl;
            cout << "2. Remove Task" << endl;
            cout << "3. Print Tasks" << endl;
            cout << "4. Exit" << endl;

            char choice;
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice)
            {
            case '1':
            {
                int priority, task_id;
                string task_name;
                Date deadline;
                do
                {
                    cout << "Enter Priority (1-9): ";
                    cin >> priority;
                } while (priority > 9);

                while (true)
                {
                    cout << "Enter Task ID (Numbers only): ";

                    try
                    {
                        if (!(cin >> task_id))
                        {
                            throw invalid_argument("Invalid input. Please enter an integer.");
                        }

                        break; // Exit the loop if input is successful
                    }
                    catch (const invalid_argument &e)
                    {
                        cerr << "Error: " << e.what() << endl;

                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                }

                cout << "Enter Task Name: ";
                cin.ignore();
                getline(cin, task_name);

                while (true)
                {
                    try
                    {
                        int day, month, year;

                        cout << "Enter Deadline (day month year ) separated by space: ";
                        cin >> day >> month >> year;
                        deadline = Date(day, month, year);
                        cout << "Valid date entered. Deadline set." << endl;
                        break;
                    }
                    catch (const invalid_argument &e)
                    {
                        cerr << "Error: " << e.what() << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please try again." << endl;
                    }
                }

                task newTask(priority, task_id, task_name, deadline);
                user.addTask(newTask);
                break;
            }

            case '2':
            {
                if (!user.empty())
                {
                    int task_id;
                    cout << "Enter Task ID to remove: ";
                    cin >> task_id;
                    user.removeTask(task_id);
                }
                else
                {
                    cout << "No tasks to remove." << endl;
                }
                break;
            }

            case '3':
                if (!user.empty())
                {
                    user.printTasks();
                }
                else
                {
                    cout << "No tasks to print." << endl;
                }
                break;

            case '4':
                cout << "Exiting program." << endl;
                return 0;

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
            }
        }
    }
    catch (exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown exception occurred." << endl;
    }

    return 0;
}