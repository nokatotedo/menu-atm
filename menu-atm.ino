#include <Keypad.h>
#include <string.h>
#include "U8glib.h"

//pin
U8GLIB_ST7920_128X64_4X u8g(10);
const byte baris = 4;
const byte kolom = 4;
char tombol[baris][kolom] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte pinBaris[baris] = {6,7,8,9};
byte pinKolom[kolom] = {A3,A2,A1,A0};
Keypad kypd = Keypad(makeKeymap(tombol),pinBaris,pinKolom,baris,kolom);
//pin

//info
char pinNasabah[2][6] = {
  {'1','2','3','4','5','6'},
  {'2','3','4','5','6','7'}
};
char namaNasabah[2][10] = {
  {"AISYAH"},
  {"PANDU"}
};
float saldoNasabah[2] = {
  10000000,
  60000
};
//info

char pilihBahasa, charPin;
char inputPin[6];
bool whilePin, whileInfo, whileSaldo, whileYakin, betulPinIndonesia, betulPinEnglish, betulInfoIndonesia, betulInfoEnglish, betulSaldoIndonesia, betulSaldoEnglish;
bool whileLanjut, betulOkeIndonesia, betulOkeEnglish, gagalOkeIndonesia, gagalOkeEnglish, betulLanjutIndonesia, betulLanjutEnglish;
bool kembaliKelompok;
bool lanjutKelompok;
int idArray, bahasa, kesempatanJawab, pin;
int idNasabah;
float saldoTarik, uangKelompok;

void setup() {
  Serial.begin(9600);
  kembaliKelompok = false;
  lanjutKelompok = false;
  uangKelompok = 0;
}

void loop() {
  Serial.print("Kelompok Sebelum : ");
  Serial.println(kembaliKelompok);
  Serial.print("Kelompok Sesudah : ");
  Serial.println(lanjutKelompok);
  Serial.print("Uang : ");
  Serial.println(uangKelompok);
  Serial.println("");
  if(Serial.available()) {
    char tes = Serial.read();
    if(tes == '1') {
      idNasabah = 1;
    } else if(tes == '2') {
      idNasabah = 2;
    }
    uangKelompok = 0;
    kembaliKelompok = true;
    lanjutKelompok = false;
  }
  
  idArray = idNasabah - 1;
  if(kembaliKelompok) {
    Serial.print("Kelompok Sebelum : ");
    Serial.println(kembaliKelompok);
    Serial.print("Kelompok Sesudah : ");
    Serial.println(lanjutKelompok);
    Serial.print("Uang : ");
    Serial.println(uangKelompok);
    Serial.println("");
    lcdBegin(1);
    clearPin();
    pilihBahasa = 0;
    kesempatanJawab = 0;
    
    char kypdBahasa = kypd.getKey();
    if(kypdBahasa) {
      pilihBahasa = kypdBahasa;
    }
  }

  if(pilihBahasa == 'A') {
    betulPinIndonesia = false;
    bahasa = 1;
    pin = 1;
    whilePin = true;
    lcdBegin(2);
    while(whilePin) {
      jawabPin();
    }
  } else if(pilihBahasa == 'B') {
    betulPinEnglish = false;
    bahasa = 2;
    pin = 1;
    whilePin = true;
    lcdBegin(3);
    while(whilePin) {
      jawabPin();
    }
  } else if(pilihBahasa == 'C') {
    lcdMati();
  }

  if(betulPinIndonesia) {
    betulInfoIndonesia = false;
    whileInfo = true;
    lcdBegin(4);
    while(whileInfo) {
      jawabInfo();
    }
  } else if(betulPinEnglish) {
    betulInfoEnglish = false;
    whileInfo = true;
    lcdBegin(5);
    while(whileInfo) {
      jawabInfo();
    }
  }

  if(betulInfoIndonesia) {
    betulSaldoIndonesia = false;
    saldoTarik = 0;
    whileSaldo = true;
    lcdBegin(6);
    while(whileSaldo) {
      jawabSaldo();
    }
  } else if(betulInfoEnglish) {
    betulSaldoEnglish = false;
    saldoTarik = 0;
    whileSaldo = true;
    lcdBegin(7);
    while(whileSaldo) {
      jawabSaldo();
    }
  }

  if(betulSaldoIndonesia) {
    whileYakin = true;
    lcdBegin(8);
    while(whileYakin) {
      jawabYakin();
    }
  } else if(betulSaldoEnglish) {
    whileYakin = true;
    lcdBegin(9);
    while(whileYakin) {
      jawabYakin();
    }
  }

  if(betulOkeIndonesia) {
    lcdBegin(10);
    uangKelompok = uangKelompok + saldoTarik;
    delay(2000);
    betulLanjutIndonesia = true;
    betulOkeIndonesia = false;
  } else if(betulOkeEnglish) {
    lcdBegin(11);
    uangKelompok = uangKelompok + saldoTarik;
    delay(2000);
    betulLanjutEnglish = true;
    betulOkeEnglish = false;
  } else if(gagalOkeIndonesia) {
    lcdBegin(12);
    uangKelompok = uangKelompok + 0;
    delay(2000);
    betulLanjutIndonesia = true;
    gagalOkeIndonesia = false;
  } else if(gagalOkeEnglish) {
    lcdBegin(13);
    uangKelompok = uangKelompok + 0;
    delay(2000);
    betulLanjutEnglish = true;
    gagalOkeEnglish = false;
  }

  if(betulLanjutIndonesia) {
    whileLanjut = true;
    lcdBegin(14);
    while(whileLanjut) {
      jawabLanjut();
    }
  } else if(betulLanjutEnglish) {
    whileLanjut = true;
    lcdBegin(15);
    while(whileLanjut) {
      jawabLanjut();
    }
  }
}

void menuBahasa() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(27, 10, "PILIH BAHASA");
  u8g.drawStr(24, 16, "----------------");
  u8g.drawStr(30, 23, "LANGUANGE");
  u8g.drawStr( 2, 38, "A. INDONESIA");
  u8g.drawStr( 2, 48, "B. ENGLISH");
  u8g.drawStr( 2, 62, "C. BATAL");
}

void menuPinIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(22, 10, "MASUKKAN PIN");
  u8g.drawStr(35, 20, "ATM ANDA");
  u8g.drawStr(30, 41, "_ _ _ _ _ _ _ _");
  u8g.drawStr( 2, 62, "C. BATAL");
}

void menuPinEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(24, 10, "PLEASE ENTER");
  u8g.drawStr(38, 20, "YOUR PIN");
  u8g.drawStr(30, 41, "_ _ _ _ _ _ _ _");
  u8g.drawStr( 2, 62, "C. CANCEL");
}

void menuInfoIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(38, 10, "SELAMAT");
  u8g.drawStr(41, 20, "DATANG");
  u8g.drawStr( 2, 38, namaNasabah[idArray]);
  u8g.drawStr( 2, 48, "RP.");
  char saldoStringNasabah[20];
  dtostrf(saldoNasabah[idArray], 8, 2, saldoStringNasabah);
  u8g.drawStr( 19, 48, saldoStringNasabah);
  u8g.drawStr( 2, 62, "C. BATAL");
  u8g.drawStr(97, 62, "A. OK");
}

void menuInfoEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(37, 10, "WELCOME");
  u8g.drawStr(50, 20, "BACK");
  u8g.drawStr( 2, 38, namaNasabah[idArray]);
  u8g.drawStr( 2, 48, "RP.");
  char saldoStringNasabah[20];
  dtostrf(saldoNasabah[idArray], 8, 2, saldoStringNasabah);
  u8g.drawStr( 19, 48, saldoStringNasabah);
  u8g.drawStr( 2, 62, "C. CANCEL");
  u8g.drawStr(97, 62, "A. OK");
}

void menuSaldoIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(27, 10, "PILIH JUMLAH");
  u8g.drawStr(13, 20, "PENARIKAN SALDO");
  u8g.drawStr( 2, 38, "A. RP.50.000");
  u8g.drawStr( 2, 48, "B. RP.100.000");
  u8g.drawStr( 2, 62, "C. BATAL");
}

void menuSaldoEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(20, 10, "PLEASE CHOOSE");
  u8g.drawStr(40, 20, "AMOUNT");
  u8g.drawStr( 2, 38, "A. RP.50.000");
  u8g.drawStr( 2, 48, "B. RP.100.000");
  u8g.drawStr( 2, 62, "C. CANCEL");
}

void menuYakinIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(24, 27, "APAKAH ANDA");
  u8g.drawStr(44, 37, "YAKIN?");
  u8g.drawStr( 2, 62, "C. BATAL");
  u8g.drawStr(97, 62, "A. OK");
}

void menuYakinEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(40, 27, "ARE YOU");
  u8g.drawStr(48, 37, "SURE?");
  u8g.drawStr( 2, 62, "C. CANCEL");
  u8g.drawStr(97, 62, "A. OK");
}

void menuOkeIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(26, 36, "TERIMA KASIH");
}

void menuOkeEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(32, 36, "THANK YOU");
}

void menuGagalIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(8, 36, "SALDO TIDAK CUKUP");
}

void menuGagalEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(28, 31, "INSUFFICIENT");
  u8g.drawStr(39, 41, "BALANCE");
}

void menuLanjutIndonesia() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(7, 32, "LANJUT TRANSAKSI?");
  u8g.drawStr( 2, 62, "C. BATAL");
  u8g.drawStr(97, 62, "A. OK");
}

void menuLanjutEnglish() {
  u8g.setFont(u8g_font_helvB08);
  u8g.drawStr(25, 27, "TRANSACTION");
  u8g.drawStr(44, 37, "AGAIN?");
  u8g.drawStr( 2, 62, "C. CANCEL");
  u8g.drawStr(97, 62, "A. OK");
}

void lcdBegin(int pilih) {
  u8g.firstPage();
  do {
    lcdPrint(pilih);
  } while(u8g.nextPage());
}

void lcdPrint(int pilih) {
  switch(pilih) {
    case 1:
    menuBahasa();
    break;

    case 2:
    menuPinIndonesia();
    break;

    case 3:
    menuPinEnglish();
    break;

    case 4:
    menuInfoIndonesia();
    break;

    case 5:
    menuInfoEnglish();
    break;

    case 6:
    menuSaldoIndonesia();
    break;

    case 7:
    menuSaldoEnglish();
    break;

    case 8:
    menuYakinIndonesia();
    break;

    case 9:
    menuYakinEnglish();
    break;

    case 10:
    menuOkeIndonesia();
    break;

    case 11:
    menuOkeEnglish();
    break;

    case 12:
    menuGagalIndonesia();
    break;

    case 13:
    menuGagalEnglish();
    break;

    case 14:
    menuLanjutIndonesia();
    break;

    case 15:
    menuLanjutEnglish();
    break;
  }
}

void lcdPinIndonesia(int banyakPin) {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_helvB08);
    u8g.drawStr(22, 10, "MASUKKAN PIN");
    u8g.drawStr(35, 20, "ATM ANDA");
    u8g.drawStr( 2, 62, "C. BATAL");
    if(banyakPin == 1) {
      u8g.drawStr(30, 41, "X");
    } else if(banyakPin == 2) {
      u8g.drawStr(30, 41, "X X");
    } else if(banyakPin == 3) {
      u8g.drawStr(30, 41, "X X X");
    } else if(banyakPin == 4) {
      u8g.drawStr(30, 41, "X X X X");
    } else if(banyakPin == 5) {
      u8g.drawStr(30, 41, "X X X X X");
    } else if(banyakPin == 6) {
      u8g.drawStr(30, 41, "X X X X X X");
    }
  } while(u8g.nextPage());
}

void lcdPinEnglish(int banyakPin) {
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_helvB08);
    u8g.drawStr(24, 10, "PLEASE ENTER");
    u8g.drawStr(38, 20, "YOUR PIN");
    u8g.drawStr( 2, 62, "C. CANCEL");
    if(banyakPin == 1) {
      u8g.drawStr(30, 41, "X");
    } else if(banyakPin == 2) {
      u8g.drawStr(30, 41, "X X");
    } else if(banyakPin == 3) {
      u8g.drawStr(30, 41, "X X X");
    } else if(banyakPin == 4) {
      u8g.drawStr(30, 41, "X X X X");
    } else if(banyakPin == 5) {
      u8g.drawStr(30, 41, "X X X X X");
    } else if(banyakPin == 6) {
      u8g.drawStr(30, 41, "X X X X X X");
    }
  } while(u8g.nextPage());
}

void lcdMati() {
  betulPinIndonesia = false;
  betulPinEnglish = false; 
  betulInfoIndonesia = false;
  betulInfoEnglish = false; 
  betulSaldoIndonesia = false;
  betulSaldoEnglish = false;
  betulOkeIndonesia = false;
  betulOkeEnglish = false;
  gagalOkeIndonesia = false;
  gagalOkeEnglish = false;
  betulLanjutIndonesia = false;
  betulLanjutEnglish = false;
  
  kembaliKelompok = false;
  lanjutKelompok = true;
  pilihBahasa = 0;
  u8g.firstPage();
  do {
  } while(u8g.nextPage());
  Serial.print("Kelompok Sebelum : ");
  Serial.println(kembaliKelompok);
  Serial.print("Kelompok Sesudah : ");
  Serial.println(lanjutKelompok);
  Serial.print("Uang : ");
  Serial.println(uangKelompok);
  Serial.println("");
}

void clearPin() {
  for(charPin = 0; charPin <= 6; charPin++) {
    inputPin[charPin] = 0;
  }
  charPin = 0;
}

void jawabPin() {
  char kypdPin = kypd.getKey();
  if(charPin == 6) {
    clearPin();
    pin = 1;
    kesempatanJawab++;
  }
  
  if(kypdPin) {
    if(kypdPin == 'C') {
      whilePin = false;
    } else if((((((kypdPin == 'B')||(kypdPin == 'A'))||kypdPin == 'D')||kypdPin == '*')||kypdPin == '#')) {
      
    } else {
      if(bahasa == 1) {
        lcdPinIndonesia(pin);
      } else if(bahasa == 2) {
        lcdPinEnglish(pin);
      }
      inputPin[charPin] = kypdPin;
      charPin++;
      pin++;
      whilePin = true;
    }
  }

  cekPin();
  
  if(kesempatanJawab == 3) {
    whilePin = false;
    lcdMati();
  }
}

void cekPin() {
  if((((((inputPin[0] == pinNasabah[idArray][0])&&(inputPin[1] == pinNasabah[idArray][1]))&&(inputPin[2] == pinNasabah[idArray][2]))&&(inputPin[3] == pinNasabah[idArray][3]))&&(inputPin[4] == pinNasabah[idArray][4]))&&(inputPin[5] == pinNasabah[idArray][5])) {
    if(bahasa == 1) {
      betulPinIndonesia = true;
    } else if(bahasa == 2) {
      betulPinEnglish = true;
    }
    whilePin = false;
  }
}

void jawabInfo() {
  char kypdInfo = kypd.getKey();
  if(kypdInfo) {
    if(kypdInfo == 'C') {
      betulPinIndonesia = false;
      betulPinEnglish = false;
      whileInfo = false;
    } else if(kypdInfo == 'A') {
      if(bahasa == 1) {
       betulInfoIndonesia = true; 
      } else if(bahasa == 2) {
        betulInfoEnglish = true; 
      }
      whileInfo = false;
    }
  }
}

void jawabSaldo() {
  char kypdSaldo = kypd.getKey();
  if(kypdSaldo) {
    if(kypdSaldo == 'A') {
      saldoTarik = 50000;
    } else if(kypdSaldo == 'B') {
      saldoTarik = 100000;
    } else if(kypdSaldo == 'C') {
      betulInfoEnglish = false;
      betulInfoIndonesia = false;
      whileSaldo = false;
    }
  }
  
  if(saldoTarik >= 50000) {
    if(bahasa == 1) {
      betulSaldoIndonesia = true;
    } else if(bahasa == 2) {
      betulSaldoEnglish = true;
    }
    whileSaldo = false;
  }
}

void jawabYakin() {
  char kypdYakin = kypd.getKey();
  if(kypdYakin) {
    if(kypdYakin == 'A') {
      float saldo = saldoNasabah[idArray] - saldoTarik;
      if(saldo > 0) {
        saldoNasabah[idArray] = saldo;
        if(bahasa == 1) {
          betulOkeIndonesia = true;
        } else if(bahasa == 2) {
          betulOkeEnglish = true;
        }
      } else {
        if(bahasa == 1) {
          gagalOkeIndonesia = true;
        } else if(bahasa == 2) {
          gagalOkeIndonesia = true;
        }
      }
      betulSaldoIndonesia = false;
      betulSaldoEnglish = false;
      whileYakin = false;
    } else if(kypdYakin == 'C') {
      betulSaldoIndonesia = false;
      betulSaldoEnglish = false;
      whileYakin = false;
    }
  }
}

void jawabLanjut() {
  char kypdLanjut = kypd.getKey();
  if(kypdLanjut) {
    if(kypdLanjut == 'A') {
      betulLanjutIndonesia = false;
      betulLanjutEnglish = false;
      whileLanjut = false;
    } else if(kypdLanjut == 'C') {
      whileLanjut = false;
      lcdMati();
    }
  }
}
