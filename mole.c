/*
  Program BANANA MOLE
  File    mole.c
  Create  2024/05/11
  Update  2024/06/06
  Version 0.9.1.0
  By      Kenoh
*/
#include "extlib.h"
#include "stage.h"
#define MAX_X 18
#define MAX_Y 6
#define MAX_STAGE 20
#define C_BLUNK 0 /* ¸³Ê¸ */
#define C_SOIL 1 /* ÂÁ */
#define C_ROCK 2 /* ²Ü */
#define C_LADDER 3 /* Ê¼ºÞ */
#define C_HOUSE 4 /* ²´ */
#define C_FOOD 5 /* ÀÍÞÓÉ */
#define S_CLEAR 1 /* ½Ã°¼Þ¸Ø± */
#define S_MISS 2 /* Ð½ */
#define S_OVER 3 /* ¹Þ°Ñµ°ÊÞ° */
#define S_END 4 /* ´ÝÃÞ¨Ý¸Þ */
#define ON "ON "
#define OFF "OFF"
const char *C_player[]={
  "88FE7B7BFE88",
  "08FE3F7BFE92",
  "92FE7B3FFE08",
};
const char *C_etc="7E81BDA5817E";
const char *C_map[]={
  "000000000000",
  "050A210A1502",
  "7EFFDFFBFF7E",
  "FF55555555FF",
  "FC92939392FC",
  "705C7E3E1B0F",
  "60E0F0793F0D",
  "0C7AD7AB760C",
  "E07F7D39111E",
  "7987B5B58779",
  "28023C3C0052",
};
unsigned char Stage=1;
int Player;
unsigned long Score=0;
unsigned long HiScore=0;
unsigned int Step;
char Food;
char Sound=0;
int Waittime=500;
/* Ï¯ÌßÁ¯Ìß ËÞ®³¶Þ */
void map_put(int x,int y,char chip) {
  gcursor(x*6,y*8+8-1);
  gprint(C_map[chip]);
}
/* Ï¯Ìß ËÞ®³¶Þ */
void map_draw(char *map,int mline,int mlen) {
  int x,y;
  char w;
  Food=0;
  for(y=0;y<mline;y++) {
    for(x=0;x<mlen;x++) {
      w=(char)*(map+y*mlen+x);
      if(w>=C_FOOD)
        Food++;
      if(Sound)
        beep(200,5,1);
      map_put(x,y,w);
    }
  }
  gotoxy(19,0);
  printf("STAGE");
  gotoxy(21,1);
  printf("%02d",Stage);
  gotoxy(19,2);
  printf("STEPS");
  gotoxy(19,3);
  printf("%4d",Step);
  gotoxy(19,4);
  printf("MOLE");
  for(w=Player-1;w>0;w--) {
    gcursor(23*6-w*6,5*8+8-1);
    gprint(*C_player);
  }
}
/* ½Ã°À½ ËÞ®³¶Þ */
void status_draw() {
  Step++;
  gotoxy(20,3);
  printf("%3d",Step);
}
/* ½Ìß×²Ä ²ÄÞ³ ¼®Ø */
void sprite_move(int x,int y,int x1,int y1,char **obj,unsigned char pattern,unsigned char *wk,unsigned char wkpattern,char *map,int mlen,unsigned char *bgm,int bwait,int *bp) {
  int i,i1,wx,wy;
  i1=6*(x1!=0)+8*(y1!=0);
  for(i=1;i<=i1;i++) {
    map_put(x,y,(int)*(map+y*mlen+x));
    wx=x*6+i*x1;
    wy=y*8+8+i*y1;
    gcursor(wx,wy-1);
    gprint(obj[*wk+pattern]);
    *wk=(++*wk)%wkpattern;
    if(x1==-1) {
      wx+=i1;
    } else if(x1==1) {
      wx-=1;
    } else if(y1==-1) {
      wy+=1;
    } else if(y1==1) {
      wy-=i1;
    }
    if(Sound) {
      bwait+=(bwait<1);
      beep(*(bgm+*bp),bwait,1);
      *bp=(++*bp)%(sizeof(bgm));
    } else {
        wait(Waittime/i1);
    }
  }
}
/* ÌßÚ²Ô° ²ÄÞ³ ¼®Ø */
void player_move(int *x,int *y,int x1,int y1,char *map,int mline,int mlen) {
  static unsigned char wk=0;
  static int bp=0;
  const unsigned char bgm[4]={219,143,172,194};
  sprite_move(*x,*y,x1,y1,C_player,0,&wk,3,map,mlen,bgm,Waittime/50,&bp);
  *x+=x1;
  *y+=y1;
  status_draw();
}
/* ²Ü ×¯¶ ¼®Ø */
void rock_move(int x,int y,int px,char *map,int mline,int mlen) {
  int iy1,iy2; 
  static unsigned char wk=0;
  static int bp=0;
  const unsigned char bgm[5]={143,172,219,194,232};
  /* ¼Ã²¼À X¥Y-1¼Þ¸ É ²Ü ¦ ×¯¶ */
  if(y>0) {
    for(iy1=y-1;iy1>=0;iy1--) {
      if((int)*(map+iy1*mlen+x)==C_ROCK&&(int)*(map+(iy1+1)*mlen+x)==C_BLUNK) {
        /* ¸³Ê¸ ¶Þ Å¸ÅÙÏÃÞ ²Ü ×¯¶ */
        for(iy2=iy1+1;iy2<MAX_Y;iy2++) {
          if(*(map+iy2*mlen+x)==C_BLUNK&&!(px==x&&iy2==y)) {
            *(map+iy2*mlen+x)=C_ROCK;
            *(map+(iy2-1)*mlen+x)=C_BLUNK;
            sprite_move(x,iy2-1,0,1,C_map,C_ROCK,&wk,1,map,mlen,bgm,Waittime/50,&bp);
          } else {
            break;
          }
        }
        rock_move(x,iy1,x,map,mline,mlen);
      }
    }
  }
}
/* ÌßÚ²Ô° ²ÄÞ³ Áª¯¸ */
void player_move_chk(int key,int x,int y,int *x1,int *y1,char *map,int mline,int mlen) {
  int wx,wy;
  char *mp;
  /* ·°¿³»¥²ÄÞ³ÊÝ² Áª¯¸ */
  *x1=-(key==0x34)+(key==0x36);
  *y1=-(key==0x38)+(key==0x32);
  wx=x+*x1;
  *x1*=(wx>=0)&&(wx<mlen);
  wy=y+*y1;
  *y1*=(wy>=0)&&(wy<mline);
  /* ²ÄÞ³»· Áª¯¸ */
  /* Ê¼ºÞ ¼®³º³Á­³ ÉÐ ³´ ²ÄÞ³ ¶É³ */
  mp=map+y*mlen+x;
  if((*y1==-1)&&*mp!=C_LADDER) {
    *x1=0;
    *y1=0;
  } else if(*x1||*y1) {
    mp=map+wy*mlen+wx;
    switch(*mp) {
      case C_SOIL:
        *mp=C_BLUNK;
        break;
      case C_ROCK:
        *x1=0;
        *y1=0;
        break;
      default:
        if(*mp>=C_FOOD) {
          Food--;
          Score+=(unsigned long)*mp*100;
          *mp=C_BLUNK;
          if(Sound) {
            beep(232,15,1);
            beep(127,15,1);
          }
        }
        break;
    }
  }
}
/* ¼Þ®³·®³ Áª¯¸ */
char status_chk(int key,int x,int y,char *map,int mlen) {
  char clrSw=NULL;
  if(key==0x20) {
    Player--;
    if (Player>0) {
      clrSw=S_MISS;
    } else {
      clrSw=S_OVER;
    }
  } else {
    /* ²´ Æ ²ÄÞ³¼ ½ÍÞÃÉ ÀÍÞÓÉ ¦ ¼­Ä¸¼ÀÊÞ±² ¸Ø± */
    if(*(map+y*mlen+x)==C_HOUSE&&Food<=0) {
      Score+=1000000/(unsigned long)Step;
      Stage++;
      /* »²ÀÞ²½Ã°¼Þ ¦ º´À× ´ÝÃÞ¨Ý¸Þ */
      if (Stage<=MAX_STAGE) {
        clrSw=S_CLEAR;
      } else {
        clrSw=S_END;
      }
    }
  }
  return clrSw;
}
/* À²ÄÙ */
void title() {
  int key;
  /* Ë®³¼Þ */
  clrscr();
  map_put(0,0,C_FOOD);
  map_put(2,0,C_FOOD+1);
  map_put(4,0,C_FOOD+2);
  gotoxy(6,0);
  printf("BANANA MOLE");
  map_put(18,0,C_FOOD+3);
  map_put(20,0,C_FOOD+4);
  map_put(22,0,C_FOOD+5);
  gotoxy(0,2);
  printf("SPACE=START");
  gotoxy(0,3);
  printf("82=STAGE ");
  printf("%2d",Stage);
  gotoxy(13,2);
  printf("S=SOUND");
  gotoxy(21,2);
  if(Sound) {
    printf(ON);
  } else {
    printf(OFF);
  }
  gotoxy(13,3);
  printf("+-=SPEED ");
  printf("%2d",100-Waittime/10);
  gcursor(6*6,5*8+8-1);
  gprint(C_etc);
  gotoxy(8,5);
  printf("2024 Kenoh");
  do {
    key=getch();
    if(toupper(key)==0x53) {
      /* »³ÝÄÞ ·Ø¶´ */
      Sound=!Sound;
      gotoxy(21,2);
      if(Sound) {
        printf(ON);
        beep(49,80,1);
        beep(30,200,1);
      } else {
        printf(OFF);
      }
    } else if(key==0x2b||key==0x2d) {
      /* ÏÁ¼Þ¶Ý ¾¯Ã² */
      Waittime+=(-(key==0x2b)+(key==0x2d))*10;
      if(Waittime<10) {
        Waittime=10;
      }
      if(Waittime>990) {
        Waittime=990;
      }
      gotoxy(22,3);
      printf("%2d",100-Waittime/10);
    } else if(key==0x32||key==0x38) {
      /* ½Ã°¼Þ ¾ÝÀ¸ */
      Stage+=(-(key==0x32)+(key==0x38));
      if(Stage<1) {
        Stage=1;
      }
      if(Stage>MAX_STAGE) {
        Stage=MAX_STAGE;
      }
      gotoxy(9,3);
      printf("%2d",Stage);
    }
  } while(key!=0x20);
  /* ¼®·¾¯Ã² */
  Score=0;
  Player=3;
  HiScore=(unsigned long)peek(0x00fd)*0x10000+(unsigned long)peek(0x00fe)*0x100+(unsigned long)peek(0x00ff);
}
/* ¹Þ°Ñ */
char game() {
  int key;
  int x=0;
  int y=0;
  int x1,y1;
  char statusSw=0;
  const unsigned char s_bgm[6]={112,112,82,72,60,52};
  const unsigned char c_bgm[11]={105,93,82,77,68,60,52,49,60,105,93};
  const unsigned char g_bgm[13]={105,93,105,127,143,127,105,127,143,162,127,127,127};
  char *map=malloc(sizeof(char)*(MAX_Y*MAX_X));
  /* ¼®·Ë®³¼Þ */
  clrscr();
  stageRead(map,Stage);
  map_draw(map,MAX_Y,MAX_X);
  gcursor(0,8-1);
  gprint(*C_player);
  Step=0;
  if(Sound)
    bgm_play(s_bgm,sizeof(s_bgm)/sizeof(char),16);
  /* ¹Þ°ÑÒ²Ý */
  do {
    /* ¿³»ÊÝÃ² */
    key=inkey();
    player_move_chk(key,x,y,&x1,&y1,map,MAX_Y,MAX_X);
    if(x1||y1) {
      player_move(&x,&y,x1,y1,map,MAX_Y,MAX_X);
      rock_move(x-x1,y,x,map,MAX_Y,MAX_X);
      statusSw = status_chk(key,x,y,map,MAX_X);
      if(statusSw)
        break;
      /* ÌßÚ²Ô° ×¯¶ ¼®Ø */
      for(y1=y+1;y1<MAX_Y;y1++) {
        if(*(map+y*MAX_X+x)!=C_LADDER&&*(map+y1*MAX_X+x)==C_BLUNK) {
          player_move(&x,&y,0,1,map,MAX_Y,MAX_X);
          rock_move(x,y,x,map,MAX_Y,MAX_X);
          statusSw = status_chk(key,x,y,map,MAX_X);
          if(statusSw)
            break;
        } else {
          break;
        }
      }
    }
    /* ½Ã°¼Þ¸Ø±¥·ÞÌÞ±¯Ìß ÏÃÞ ¹²¿Þ¸ */
    if(statusSw==NULL)
      statusSw = status_chk(key,x,y,map,MAX_X);
  } while(!statusSw);
  /* ½Ã°À½ Ë®³¼Þ */
  switch(statusSw) {
    case S_MISS:
    case S_OVER:
      gotoxy(4,0);
      printf(" GIVE UP ! ");
      if(Sound)
        bgm_play(g_bgm,sizeof(g_bgm)/sizeof(char),16);
      break;
    case S_CLEAR:
    case S_END:
      gotoxy(2,0);
      printf(" STAGE CLEAR ! ");
      if(Sound)
        bgm_play(c_bgm,sizeof(c_bgm)/sizeof(char),16);
      break;
  }
  stopEnter();
  free(map);
  return statusSw;
}
/* ¼­³Ø®³ */
void end(char statusSw) {
  int key;
  const unsigned char o_bgm[6]={105,82,93,105,127,112};
  const unsigned char e_bgm[7]={60,52,49,43,49,60,35};
  clrscr();
  gotoxy(5,3);
  printf("SCORE %7lu",Score);
  if(Score>HiScore) {
    HiScore=Score;
    poke(0x00fd,HiScore/0x10000);
    poke(0x00fe,HiScore/0x100);
    poke(0x00ff,HiScore%0x100);
  }
  gotoxy(3,4);
  printf("HISCORE %7lu",HiScore);
  switch(statusSw) {
    case S_OVER:
      gotoxy(7,1);
      printf("GAME OVER");
      if(Sound)
        bgm_play(o_bgm,sizeof(o_bgm)/sizeof(char),16);
      break;
    case S_END:
      gotoxy(3,1);
      printf("CONGRATULATIONS !!");
      if(Sound)
        bgm_play(e_bgm,sizeof(e_bgm)/sizeof(char),16);
      Stage=1;
      break;
  }
  stopEnter();
}
/* Ò²Ý */
void main() {
  char statusSw;
  while(1) {
    title();
    do {
      statusSw=game();
    } while(statusSw==S_CLEAR||statusSw==S_MISS);
    end(statusSw);
  }
}
