#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *C = "UOUQOEOOMFOOFTAMTVTTAQQHFSMUOAXHXFHRSTBENBCLXDIAUHXYIAETHEUSGEKFHRTLBZGFBNWMREPWLAFBVTKMGRPULROEUUGQOEUOMMKRBRFEATBIGETNTETAFGSONNLRTAGPBLPPIASVOGXZDGIEFFOQJEMASYFEIZOZPRXMNQCYTELRFPMASNZWXQNQUHXTENSTTOHRBNWFHRUHHGSNODGMTHSAEEHBDKLFHNUFEQSUJSAQIEUOMUSNDOGEUZNAMUOAEEOAUGMYMABRXILTEQUOWUEGPSEQECUOLXERQPXDCUBNVQTBERXMMNZEMTEEFSMTEEVBYARVOTAMTFMEXBOSEETFHJIAMPRRBMLYALDOFQWUFNPQHNWELTUSGLXPOSGTAUSZPRMMLPPIEYUFUGBHEHTPTGSRUHXDEFUHXDEFQEVFTUBTFMKRTCTXAZJTRAFFPLHZGYJFXROEXHHIOHMDUQAEUHXIHVQSTZDFDOKZSBGTBYEGIEHBPEFSLARFXRHZGGIEIDOHEMTZSPPNMGMRMYMTECBNZEOSEELBIFFDEAVRUHXXAJTDXXALUHXUNFPLXZCRPFHRFVDETZDGIELBUEOSMTAGQAMUEAUMXDIGPFMTEHOWHDTUZTTWEFXHXZHRIIFEEYGMBSHGIILCUVFTNEMNLEPUTUBBTDEOPDDUNJIOPAUYEFTDDRMSUQAEUOZDUAUAGPSJFAMGNQFRTIENSYEUFRCUMFHNUTAQDEFAWAFFPMXFHVOGTRTRSDXMTUUHXGNQJSVAVRSEWOOHOTKKFEPMPTOFFBHGRAOOMDAIFLEQREFTNDNFQUSLLRTTAQWVMLTZDZBKXEUFSAMTEECETDTUPSXULYTWXTAIFTAMNSMYMAOGIEKETUBTPQKAPWGATBGTAGSPPNLOIROCXPORTMTWEPPWTDDFPFNEAYMAGPTUVSMTEABTBHEUVEHRRRTOEGTVPNBESVDKEUEQPEKIIGITAQPNMEVMSGPFMTOHHHMMNQFNMQRCSILQSBGGKQAGQIMOHNODFAMROTPUTUUHBERRHAKPTUFIKOUESEGFSGVRGMWEZAGPLBTEMTEABMXAFNDTBANFPFMKOHOOPFHRGABDOCIEEUAAZMITIAUHRARVTOGEBRBLEYYFJNLDEZFMUQRRE";
int C_len = strlen(C);

// 按密钥长度分割密文
char **get_m(int key_len)
{
    char **messages = (char **)malloc(key_len * sizeof(char *));
    for (int i = 0; i < key_len; i++)
    {
        messages[i] = (char *)malloc((C_len / key_len + 1) * sizeof(char));
        memset(messages[i], 0, (C_len / key_len + 1) * sizeof(char));
    }
    for (int i = 0; i < C_len; i++)
    {
        messages[i % key_len][i / key_len] = C[i];
    }
    return messages;
}

void destroy(char **messages)
{
    free(messages);
}

// 统计各字母出现次数
int *count(char *message)
{
    int *cnt = (int *)malloc(sizeof(int) * 26);
    memset(cnt, 0, sizeof(int) * 26);
    for (int i = 0; i < strlen(message); i++)
    {
        cnt[message[i] - 'A']++;
    }
    return cnt;
}

// 计算重合指数
double calculate_index(char *message)
{
    int *cnt = count(message);
    int n = strlen(message);
    int sum = 0;
    for (int i = 0; i < 26; i++)
    {
        sum += cnt[i] * (cnt[i] - 1);
    }
    return (double)sum / (n * (n - 1));
}

// 输出字符串中各字母出现次数
void printcount(char *message)
{
    int *cnt = count(message);
    int n = strlen(message);
    for (int i = 0; i < 26; i++)
    {
        printf("%c:%.3lf  ", i + 'A', (double)cnt[i] / n);
    }
    printf("\n");
}

// 打印结果：重合指数
void printpart(int key_len, char **messages)
{
    printf("分割后子串:\n");
    for (int i = 0; i < key_len; i++)
    {
        printf("%d\n", i + 1, messages[i], calculate_index(messages[i]));
        // printf("%s\n重合指数:%.5lf\n", messages[i], calculate_index(messages[i])); // 打印重合指数
        printcount(messages[i]); // 打印字母频数
    }
}

int main()
{
    int key_len = 6;
    char **messages = get_m(key_len);
    printpart(key_len, messages);
    destroy(messages);
    return 0;
}
