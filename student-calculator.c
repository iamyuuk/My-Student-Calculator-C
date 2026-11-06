#include <stdio.h>
#include <string.h>

#define MAX_LENTH 50
#define MAX_SUBJECTS 10

typedef struct {
    char name[MAX_LENTH];
    float marks[MAX_SUBJECTS];
    char subject_names[MAX_SUBJECTS][MAX_LENTH];
    int num_subjects;
} Student;

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
        while (getchar() != '\n');
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
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            return mark;
        }
        printf("\n请输入1~100的分数！");
        while (getchar() != '\n');
    }
}

void printGrades(const Student *s) {
    int num = s->num_subjects;
    for (int i = 0;i < num;i++) {
        printf("%s的成绩为: %.2f\n", s->subject_names[i], s->marks[i]);
    }
}

float getAverage(const Student *s) {
    float avg, sum = 0;
    for (int i = 0;i<s->num_subjects;i++) {
        sum += s->marks[i];
    }
    avg = sum / s->num_subjects;
    printf("平均成绩为: %.2f\n", avg);
    return avg;
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
    if (avg >= 80) {printf("评分: A\n");}
    else if (avg >= 70) {printf("评分: B\n");}
    else if (avg >= 60) {printf("评分: C\n");}
    else {printf("评分: D\n");}
}

void printReport(const Student *s) {
    printLine();
    printf("学生成绩单\n");
    printLine();
    printf("学生姓名: %s\n", s->name);
    printLine();
    printf("科目数: %d\n", s->num_subjects);
    printLine();
    printGrades(s);
    printLine();
    float avg = getAverage(s);
    printLine();
    getMinMax(s);
    printLine();
    getGrade(avg);

}

int main() {
    char again;
    Student s;
    int ch;
    printLine();
    printf("欢迎来到学生成绩计算系统\n");
    printLine();
    do {
        printf("\n请输入姓名:");
        fgets(s.name, MAX_LENTH, stdin);
        s.name[strcspn(s.name, "\n")] = '\0';
        s.num_subjects = readSubjectNum();
        while ((ch = getchar()) != '\n' && ch != EOF);
        printf("\n接下来将输入各科名称和成绩：");
        for (int i = 0;i<s.num_subjects;i++) {
            s.marks[i] = readMarks(i + 1, &s);
        }

        printf("\n录入完成\n\n\n");
        printReport(&s);

        printf("需要输出另外一位同学的成绩单吗?(y/n):");
        scanf(" %c", &again);
        while (again != 'y' && again != 'n') printf("只能输入y或n!");
        while ((ch = getchar()) != '\n' && ch != EOF);
    } while(again == 'y');
    

    return 0;
}