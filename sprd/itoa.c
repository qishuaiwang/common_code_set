




char *itoa_simple(int num, char *str, int radix)
{
    /* \u7d22\u5f15\u8868 */
//    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char *index=0;
    index="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum; /* \u4e2d\u95f4\u53d8\u91cf */
    int i=0,j,k;
    char temp;
    /* \u786e\u5b9aunum\u7684\u503c */
    if(radix==10&&num<0) /* \u5341\u8fdb\u5236\u8d1f\u6570 */
    {
        unum=(unsigned)-num;
        str[i++]='-';
    }
    else unum=(unsigned)num; /* \u5176\u4ed6\u60c5\u51b5 */
    /* \u9006\u5e8f */
    do
    {
        str[i++]=index[unum%(unsigned)radix];
        unum/=radix;
    }while(unum);
    str[i]='\0';
    /* \u8f6c\u6362 */
    if(str[0]=='-') k=1; /* \u5341\u8fdb\u5236\u8d1f\u6570 */
    else k=0;

    for(j=k;j<=(i-k-1)/2;j++)
    {
        temp=str[j];
        str[j]=str[i-j-1];
        str[i-j-1]=temp;
    }
    return str;
}

