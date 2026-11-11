#include <stdio.h>
#include <string.h>

#define MAX_LENTH 50
#define MAX_SUBJECTS 10
#define MAX_STUDENTS 50

typedef struct {
    char name[MAX_LENTH];
    float marks[MAX_SUBJECTS];
    char subject_names[MAX_SUBJECTS][MAX_LENTH];
    int num_subjects;
    float avg;
} Student;

const float grade_boundares[] = {
    80.0f, 70.0f, 60.0f, 0
};

const char *grade_names[] = {
    "A", "B", "C", "D"
};

void clearInputBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void printLine() {
    printf("-------------------\n");
}

int readSubjectNum() {
    int n;
    while (1) {
        printf("\n请输入科目数量:");
        if( scanf("%d", &n) == 1 && n >= 1 && n <= 10) { // 这里的scanf == 1是判断n被输入了内容而不是判断n=1
            return n;
        }
        printf("\n请输入1到%d的科目数量！", MAX_SUBJECTS);
        clearInputBuffer();
    }
}

float readMarks(int subject_num, Student *s) {
    float mark;
    while (1) {
        printf("\n请输入科目%d的名称:", subject_num);
        fgets(s->subject_names[subject_num - 1], MAX_LENTH, stdin);
        s->subject_names[subject_num - 1][strcspn(s->subject_names[subject_num - 1], "\n")] = '\0';
        printf("\n请输入%s的成绩:",s->subject_names[subject_num - 1]);
        if (scanf("%f", &mark) == 1 && mark >= 1 && mark <= 100) {
            clearInputBuffer();
            return mark;
        }
        printf("\n请输入1~100的分数！");
        clearInputBuffer();
    }
}

void printGrades(const Student *s) {
    int num = s->num_subjects;
    for (int i = 0;i < num;i++) {
        printf("%s的成绩为: %.2f\n", s->subject_names[i], s->marks[i]);
    }
}

float getAverage(const Student *s) {
    float sum = 0;
    for (int i = 0;i<s->num_subjects;i++) {
        sum += s->marks[i];
    }
    return sum/ s->num_subjects;
}

void getMinMax(const Student *s) {
    float marks[MAX_SUBJECTS];
    for (int i = 0;i<s->num_subjects;i++) {
        marks[i] = s->marks[i];
    }
    // 冒泡排序
    for (int i = 0;i < s->num_subjects - 1;i++) {
        for (int j = 0;j < s->num_subjects - 1 - i;j++) {
            if (marks[j] > marks[j+1]) {
                float temp = marks[j];
                marks[j] = marks[j+1];
                marks[j+1] = temp;
            }
        }
    }
    printf("最低科目成绩为: %.2f\n", marks[0]);
    printf("最高科目成绩为: %.2f\n", marks[s->num_subjects - 1]);

}

void getGrade(float avg) {
    for (int i = 0;i < sizeof(grade_boundares) / sizeof(grade_boundares[0]);i++) {
        if (avg >= grade_boundares[i]) {
            printf("评分: %s\n", grade_names[i]);
            break;
        }
    }
}

void getTotalScore(const Student *s) {
    float sum = 0;
    for (int i = 0;i < s->num_subjects;i++) {
        sum += s->marks[i];
    }
    printf("总分为: %.2f\n", sum);
}

void printReport(const Student *s) {
    
    printf("学生姓名: %s\n", s->name);
    printLine();
    printf("科目数: %d\n", s->num_subjects);
    printLine();
    printGrades(s);
    printLine();
    getTotalScore(s);
    float avg = getAverage(s);
    printf("平均成绩为: %.2f\n", avg);
    printLine();
    getMinMax(s);
    printLine();
    getGrade(s->avg);

}

void readStudents(Student *s, int mem) {
    char again;
        printf("\n请输入学生%d姓名:", mem);
        fgets(s->name, MAX_LENTH, stdin);
        s->name[strcspn(s->name, "\n")] = '\0';
        s->num_subjects = readSubjectNum();
        clearInputBuffer();
        printf("\n接下来将输入各科名称和成绩：");
        for (int i = 0;i<s->num_subjects;i++) {
            s->marks[i] = readMarks(i + 1, s);
        }

        printf("\n录入完成\n\n\n");
        s->avg = getAverage(s);
}

void sortStudentsByAverage(Student s[], int mem) {
    for (int i = 0;i < mem - 1;i++) {
        for (int j = 0;j < mem - 1 - i;j++) {
            if (s[j].avg < s[j+1].avg) {
                Student temp = s[j];
                s[j] = s[j+1];
                s[j+1] = temp;
            }
        }
    }
}

const Student *findStudentByName(const Student s[], int count, const char *name) {
    for (int i = 0;i < count;i++) {
        if (strcmp(name, s[i].name) == 0) {
            return &s[i];
        }
    }
    return NULL;
}

void saveStudentsToFile(const Student s[], int count) {
    FILE *fp = fopen("students.txt", "w");
    if (fp == NULL) {
        printf("写入文件失败\n");
    } else {
        fprintf(fp, "%d\n", count);
        for (int i = 0;i < count;i++) {
            fprintf(fp, "%s\n", s[i].name);
            int num = s[i].num_subjects;
            fprintf(fp, "%d\n", s[i].num_subjects);
            for (int j = 0;j < num;j++) {
                fprintf(fp, "%s\n", s[i].subject_names[j]);
                fprintf(fp, "%.2f\n", s[i].marks[j]);
            }
        }
        fprintf(fp, "\n");
        fclose(fp);
    }
}

int loadStudentsFromFile(Student s[], int max_count) {
    FILE *fp = fopen("students.txt", "r");
    if (fp == NULL) { return -1; }
    char line[MAX_LENTH];
    int count;
    if (fgets(line, MAX_LENTH, fp) == NULL) {
        fclose(fp);
        return -1;
    }
    if (sscanf(line, "%d", &count) != 1) {
        fclose(fp);
        return -1;
    }
    if (count < 0 || count > max_count) {
        printf("文件中的学生人数不合法\n");
        fclose(fp);
        return -1;
    }
    for (int i = 0;i < count;i++) {
        if (fgets(s[i].name, MAX_LENTH, fp) == NULL) {
            fclose(fp);
            return -1;
        }
        s[i].name[strcspn(s[i].name, "\n")] = '\0';

        if (fgets(line, MAX_LENTH, fp) == NULL ||
        sscanf(line, "%d", &s[i].num_subjects) != 1 ||
        s[i].num_subjects < 1 ||
        s[i].num_subjects > MAX_SUBJECTS) {

        fclose(fp);
        return -1;
        }
        for (int j = 0;j < s[i].num_subjects;j++) {
            if (fgets(s[i].subject_names[j], MAX_LENTH, fp) == NULL) {
                fclose(fp);
                return -1;
            }
            s[i].subject_names[j][strcspn(s[i].subject_names[j], "\n")] = '\0';
            if (fgets(line, MAX_LENTH, fp) == NULL ||
            sscanf(line, "%f", &s[i].marks[j]) != 1) {
                fclose(fp);
                return -1;
            }
        }
        s[i].avg = getAverage(&s[i]);
    }
    fclose(fp);

    return count;
}

int clearAllData(int *studentCount) {
    FILE *fp = fopen("students.txt", "w");
    if (fp == NULL) {
        printf("文件写入失败");
        return -1;
    } else {
        fprintf(fp, "0");
        fclose(fp);
        *studentCount = 0;
        return 0;
    }
}

int main() {
//     int num;
//     printLine();
//     Student s[MAX_STUDENTS];
//     printf("欢迎来到学生成绩计算系统\n");
//     printLine();

//     Student loaded[MAX_STUDENTS];

//     int loadedCount = loadStudentsFromFile(loaded, MAX_STUDENTS);

//     if (loadedCount == -1) {
//         printf("读取学生数据失败！\n");
//     } else {
//         printf("成功读取 %d 名学生！\n", loadedCount);

//         for (int i = 0; i < loadedCount; i++) {
//         printReport(&loaded[i]);
//     }
// }


//     while (1) {
//         printf("请输入学生人数: ");
//         if (scanf("%d", &num) == 1 && num >= 1 && num <= MAX_STUDENTS) {
//             clearInputBuffer();
//             break;
//         }
//         printf("请输入1到%d之间的学生人数！", MAX_STUDENTS);
//         clearInputBuffer();
//     }
//     for (int i = 0;i<num;i++) {
//         readStudents(&s[i], i+1);
//     }
//     sortStudentsByAverage(s, num);
//     printLine();
//     printf("学生成绩单\n");
//     printLine();
//     for (int i = 0;i<num;i++) {
//         printReport(&s[i]);
//         printf("\n\n\n");
//     }
//     saveStudentsToFile(s, num);
//     if (num > 1) {
//         while (1) {
//             printf("\n是否需要查找指定学生成绩?(y/n)");
//             char n;
//             scanf(" %c", &n);
//             clearInputBuffer();
//             if (n == 'y') {
//                 char name[MAX_LENTH];
//                 printf("\n请输入要查找的学生姓名:");
//                 fgets(name, MAX_LENTH, stdin);
//                 name[strcspn(name, "\n")] = '\0';
//                 const Student *result = findStudentByName(s, num, name);
//                 if (result != NULL) {
//                     printReport(result);
//                 } else { printf("未找到该学生\n"); }
//             break;
//             } else if (n == 'n') {
//                 break;
//             } else {
//                 printf("\n请输入y或n!");
//                 continue;
//             }
//         }
//     }
//     return 0;

    int choice;
    Student s[MAX_STUDENTS];
    int studentCount = 0;
    int studentLoaded = loadStudentsFromFile(s, MAX_STUDENTS);
    if (studentLoaded != -1) {
        studentCount = studentLoaded;
    }
    printLine();
    printf("欢迎使用学生成绩管理系统\n");
    printLine();
    do {
        printf("1. 录入学生\n");
        printf("2. 查看所有学生\n");
        printf("3. 学生排序\n");
        printf("4. 姓名查找\n");
        printf("5. 保存数据\n");
        printf("6. 清除所有数据\n");
        printf("0. 退出系统\n");
        printf("请选择功能: ");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("请输入有效数字！\n");
            choice = -1;
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
                printf("录入学生\n");
                int plus;
                printf("需要录入的学生数量:");
                scanf("%d", &plus);
                clearInputBuffer();
                if (plus > 0 && plus <= (MAX_STUDENTS - studentCount)) {
                    for (int i = 0;i<plus;i++) {
                        readStudents(&s[studentCount],studentCount + 1);
                        studentCount++;
                    }
                } else { printf("已没有剩余空间可供存入学生！"); }
                break;
            case 2:
                printf("查看所有学生\n");
                if (studentCount == 0) {
                    printf("没有学生\n");
                } else {
                    for (int i = 0;i < studentCount;i++) {
                        printReport(&s[i]);
                    }
                }
                break;
            case 3:
                printf("学生排序\n");
                if (studentCount == 0) {
                    printf("没有学生\n");
                } else {
                    sortStudentsByAverage(s, studentCount);
                    printf("排序完成\n");
                }
                break;
            case 4:
                printf("姓名查找\n");
                if (studentCount == 0) {
                    printf("没有学生");
                } else {
                    char name[MAX_LENTH];
                    printf("请输入姓名:");
                    fgets(name, MAX_LENTH, stdin);
                    name[strcspn(name, "\n")] = '\0';
                    const Student *result = findStudentByName(s, studentCount, name);
                    if (result == NULL) {
                        printf("未查找到该学生\n");
                    } else {
                        printReport(result);
                    }
                }
                break;
            case 5:
                printf("保存数据\n");
                if (studentCount == 0) {
                    printf("没有学生\n");
                } else {
                    saveStudentsToFile(s, studentCount);
                }
                break;
            case 6:
                printf("清除所有数据\n");
                char confirm;
                printf("确认要清除数据吗?(y/n)");
                scanf(" %c", &confirm);
                clearInputBuffer();
                if (confirm == 'y') {
                    int a = clearAllData(&studentCount);
                    if (a == -1) {
                        printf("清除失败\n");
                    } else {
                        printf("清除成功\n");
                    }
                } else if (confirm == 'n') {
                    printf("已取消清除数据\n");
                } else {
                    printf("请输入正确字母\n");
                }
                
                break;
            case 0:
                printf("退出系统\n");
                break;
            default:
                printf("无效选项\n");
        }
    } while (choice != 0);


    return 0;
}