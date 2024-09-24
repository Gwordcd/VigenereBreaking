# Vigenère密码的破译（重合指数法）

大二密码学小作业，基于重合指数法破译Vigenère密码，分为分析密文求出密钥与解密两个程序，比较手动，有时间会考虑来个全自动脚本，我给作业报告总结粘下面了嘻嘻，附有作业原题，想了解原理或者破译过程的可以看看喵



### 题目

 以下一段密文是由Vigenère密码加密得到的，请查阅资料并破译这段密文。要求介绍破译的原理，给出密钥和明文。提示：使用重合指数法。

UOUQOEOOMFOOFTAMTVTTAQQHFSMUOAXHXFHRSTBENBCLXDIAUHXYIAETHEUSGEKFHRTLBZGFBNWMREPWLAFBVTKMGRPULROEUUGQOEUOMMKRBRFEATBIGETNTETAFGSONNLRTAGPBLPPIASVOGXZDGIEFFOQJEMASYFEIZOZPRXMNQCYTELRFPMASNZWXQNQUHXTENSTTOHRBNWFHRUHHGSNODGMTHSAEEHBDKLFHNUFEQSUJSAQIEUOMUSNDOGEUZNAMUOAEEOAUGMYMABRXILTEQUOWUEGPSEQECUOLXERQPXDCUBNVQTBERXMMNZEMTEEFSMTEEVBYARVOTAMTFMEXBOSEETFHJIAMPRRBMLYALDOFQWUFNPQHNWELTUSGLXPOSGTAUSZPRMMLPPIEYUFUGBHEHTPTGSRUHXDEFUHXDEFQEVFTUBTFMKRTCTXAZJTRAFFPLHZGYJFXROEXHHIOHMDUQAEUHXIHVQSTZDFDOKZSBGTBYEGIEHBPEFSLARFXRHZGGIEIDOHEMTZSPPNMGMRMYMTECBNZEOSEELBIFFDEAVRUHXXAJTDXXALUHXUNFPLXZCRPFHRFVDETZDGIELBUEOSMTAGQAMUEAUMXDIGPFMTEHOWHDTUZTTWEFXHXZHRIIFEEYGMBSHGIILCUVFTNEMNLEPUTUBBTDEOPDDUNJIOPAUYEFTDDRMSUQAEUOZDUAUAGPSJFAMGNQFRTIENSYEUFRCUMFHNUTAQDEFAWAFFPMXFHVOGTRTRSDXMTUUHXGNQJSVAVRSEWOOHOTKKFEPMPTOFFBHGRAOOMDAIFLEQREFTNDNFQUSLLRTTAQWVMLTZDZBKXEUFSAMTEECETDTUPSXULYTWXTAIFTAMNSMYMAOGIEKETUBTPQKAPWGATBGTAGSPPNLOIROCXPORTMTWEPPWTDDFPFNEAYMAGPTUVSMTEABTBHEUVEHRRRTOEGTVPNBESVDKEUEQPEKIIGITAQPNMEVMSGPFMTOHHHMMNQFNMQRCSILQSBGGKQAGQIMOHNODFAMROTPUTUUHBERRHAKPTUFIKOUESEGFSGVRGMWEZAGPLBTEMTEABMXAFNDTBANFPFMKOHOOPFHRGABDOCIEEUAAZMITIAUHRARVTOGEBRBLEYYFJNLDEZFMUQRRE


#### 重合指数法

- **原理**：自然语言（以英语为例）的重合指数约为 **0.065**，而且单标代换不会改变该值。

- **计算**：设含有 n 个字母的串x=x<sub>1</sub>x<sub>2</sub>...x<sub>n</sub>，则在x 中随机选择两个元素且这两个元素相同的概率为：

  f <sub>i</sub>为 26 个字母中第 i个字母在 x中出现的次数，当我们把 26 个字母的概率全部相加，得到的总的概率就是重合指数：

$$
I_{c}(x)=\frac{\sum{f_{i}\times(f_{i}-1)}}{n\times(n-1)}
$$

#### 思路

猜测密钥长度，按密钥长度分割密文，提取相同密钥字加密的密文，分割后每段密文可视为单表替换，测试其重合指数，若接近0.065，则很可能密钥长度猜测正确。



### 解答

#### 1.分割及重合指数计算：

代码如下：

```c
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
        printf("%s\n重合指数:%.5lf\n", messages[i], calculate_index(messages[i])); // 打印重合指数
        //printcount(messages[i]); // 打印字母频数
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

```

密文原始重合指数（key_len = 1）为0.04786

经过计算：当猜测密钥长度为6时，输出

![image-20240916155724438](C:\Users\Gword\AppData\Roaming\Typora\typora-user-images\image-20240916155724438.png)

各子串重合指数最为接近0.065，密钥长度极有可能为6

#### 2.按单表替换分析各子串求得密钥

- 统计各字母出现频率

![image-20240916162358486](C:\Users\Gword\AppData\Roaming\Typora\typora-user-images\image-20240916162358486.png)

- 分析推测对应明文：主要考察可能对应E的字母,并检查是否出现反常频率

  ![img](https://img2018.cnblogs.com/blog/1568101/201812/1568101-20181220145918781-1950048312.jpg)



| 密钥第i位 | 密文 | 明文 | 密钥 |
| --------- | ---- | ---- | ---- |
| 1         | F    | E    | B    |
| 2         | E    | E    | A    |
| 3         | X    | E    | T    |
| 4         | Q    | E    | M    |
| 5         | E    | E    | A    |
| 6         | R    | E    | N    |

- 密钥为**BATMAN**，有实际意义，试破解明文

#### 3.破译明文

代码如下：

```c
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *C = "UOUQOEOOMFOOFTAMTVTTAQQHFSMUOAXHXFHRSTBENBCLXDIAUHXYIAETHEUSGEKFHRTLBZGFBNWMREPWLAFBVTKMGRPULROEUUGQOEUOMMKRBRFEATBIGETNTETAFGSONNLRTAGPBLPPIASVOGXZDGIEFFOQJEMASYFEIZOZPRXMNQCYTELRFPMASNZWXQNQUHXTENSTTOHRBNWFHRUHHGSNODGMTHSAEEHBDKLFHNUFEQSUJSAQIEUOMUSNDOGEUZNAMUOAEEOAUGMYMABRXILTEQUOWUEGPSEQECUOLXERQPXDCUBNVQTBERXMMNZEMTEEFSMTEEVBYARVOTAMTFMEXBOSEETFHJIAMPRRBMLYALDOFQWUFNPQHNWELTUSGLXPOSGTAUSZPRMMLPPIEYUFUGBHEHTPTGSRUHXDEFUHXDEFQEVFTUBTFMKRTCTXAZJTRAFFPLHZGYJFXROEXHHIOHMDUQAEUHXIHVQSTZDFDOKZSBGTBYEGIEHBPEFSLARFXRHZGGIEIDOHEMTZSPPNMGMRMYMTECBNZEOSEELBIFFDEAVRUHXXAJTDXXALUHXUNFPLXZCRPFHRFVDETZDGIELBUEOSMTAGQAMUEAUMXDIGPFMTEHOWHDTUZTTWEFXHXZHRIIFEEYGMBSHGIILCUVFTNEMNLEPUTUBBTDEOPDDUNJIOPAUYEFTDDRMSUQAEUOZDUAUAGPSJFAMGNQFRTIENSYEUFRCUMFHNUTAQDEFAWAFFPMXFHVOGTRTRSDXMTUUHXGNQJSVAVRSEWOOHOTKKFEPMPTOFFBHGRAOOMDAIFLEQREFTNDNFQUSLLRTTAQWVMLTZDZBKXEUFSAMTEECETDTUPSXULYTWXTAIFTAMNSMYMAOGIEKETUBTPQKAPWGATBGTAGSPPNLOIROCXPORTMTWEPPWTDDFPFNEAYMAGPTUVSMTEABTBHEUVEHRRRTOEGTVPNBESVDKEUEQPEKIIGITAQPNMEVMSGPFMTOHHHMMNQFNMQRCSILQSBGGKQAGQIMOHNODFAMROTPUTUUHBERRHAKPTUFIKOUESEGFSGVRGMWEZAGPLBTEMTEABMXAFNDTBANFPFMKOHOOPFHRGABDOCIEEUAAZMITIAUHRARVTOGEBRBLEYYFJNLDEZFMUQRRE";

const char *KEY = "BATMAN";

char *solve()
{
    int C_len = strlen(C);
    int key_len = strlen(KEY);
    char *M = (char *)malloc(sizeof(char) * (C_len + 1));
    int i;
    for (i = 0; i < C_len; i++)
    {
        M[i] = (C[i] - KEY[i % key_len] + 26) % 26 + 'A';
    }
    M[i] = '\0';
    return M;
}

int main()
{
    char *M = solve();
    printf("%s\n", M);
    free(M);
    return 0;
}

```

运行结果：

![image-20240916164730830](C:\Users\Gword\AppData\Roaming\Typora\typora-user-images\image-20240916164730830.png)

得到可读明文，证明密钥BATMAN正确，密文破译成功，发现明文出自莎士比亚《哈姆雷特》第三幕第场片段：

***To be, or not to be, that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take arms against a sea of troubles
And by opposing end them. To die: to sleep;
No more; and by a sleep to say we end
The heart-ache and the thousand natural shocks
That flesh is heir to, 'tis a consummation
Devoutly to be wish'd. To die, to sleep;
To sleep: perchance to dream: ay, there's the rub;
For in that sleep of death what dreams may come
When we have shuffled off this mortal coil,
Must give us pause: there's the respect
That makes calamity of so long life;
For who would bear the whips and scorns of time,
The oppressor's wrong, the proud man's contumely,
The pangs of despised love, the law's delay,
The insolence of office and the spurns
That patient merit of the unworthy takes,
When he himself might his quietus make
With a bare bodkin? Who would fardels bear,
To grunt and sweat under a weary life,
But that the dread of something after death,
The undiscover'd country from whose bourn
No traveller returns, puzzles the will
And makes us rather bear those ills we have
Than fly to others that we know not of?
Thus conscience does make cowards of us all;
And thus the native hue of resolution
Is sicklied o'er with the pale cast of thought,
And enterprises of great pitch and moment
With this regard their currents turn awry,
And lose the name of action. Soft you now,
The fair Ophelia! Nymph, in thy orisons
Be all my sins remember'd.***

至此Vigenère密文破解成功



