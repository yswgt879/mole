/* µ»ÚÀ ·° É Å²Ö³ ¦ ÖÐºÑ*/
int inkey() {
  static char *p="ÍS¾ÍV¾oÉ";
  return call((unsigned)p,0);
}
/* ¼Ã²¼À ¼Þ¶Ý ÃÞ ÌßÛ¸Þ×Ñ ¦ Ã²¼ */
void wait(int t) {
  int i;
  for(i=0;i<t;i++) {
  }
}
/* ENTER¥SPACE µ³¶ÏÃÞ Ã²¼ */
void stopEnter() {
  int key;
  do {
    key=getch();
  } while(key!=0x0a&&key!=0x20);
}
/* BGM »²¾² */
void bgm_play(unsigned char *bgm,int len,int temp) {
  char i;
  temp*=2;
  for(i=0;i<len;i++) {
    beep(*(bgm+i),5200000/temp/(166+22**(bgm+i)),1);
    wait(3000/temp);
  }
}
