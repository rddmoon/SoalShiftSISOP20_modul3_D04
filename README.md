# Laporan Penjelasan dan Penyelesaian Praktikum Sistem Operasi 2020
# Modul 3
## Kelompok D04
1. Michael Ricky (05111840000078)
2. Yaniar Pradityas Effendi (05111840000047)
# 3. Soal Nomor 2
Link ke file yang dibuat:
* [tapclient.c](https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal2/tapclient.c) - Tap Client Side
* [tapserver.c](https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal2/tapserver.c) - Tap Server Side

## Soal
Qiqi adalah sahabat MamMam dan Kaka. Qiqi , Kaka dan MamMam sangat senang
bermain “Rainbow six” bersama-sama , akan tetapi MamMam sangat Toxic ia selalu
melakukan Team killing kepada Qiqi di setiap permainannya. Karena Qiqi orang yang
baik hati, meskipun marah Qiqi selalu berkata “Aku nggk marah!!”. Kaka ingin
meredam kemarahan Qiqi dengan membuatkannya sebuah game yaitu TapTap
Game. akan tetapi Kaka tidak bisa membuatnya sendiri, ia butuh bantuan mu. Ayo!!
Bantu Kaka menenangkan Qiqi.
TapTap Game adalah game online berbasis text console. Terdapat 2 program yaitu
tapserver.c dan tapplayer.c
Syarat :
- Menggunakan Socket, dan Thread
Hint :
- fwrite, fread
Spesifikasi Game :

### CLIENT SIDE
Screen 1 :
1. Login
2. Register
Choices : {your input}

★ Pada screen 1 kalian dapat menginputkan “login”, setelah menekan enter
anda diminta untuk menginputkan username dan password seperti berikut
Screen 1 :
1. Login
2. Register
Choices : login
Username : { ex : qiqi }
Password : { ex : aku nggak marah!! }

★ Jika login berhasil maka akan menampilkan pesan “login success”, jika gagal
akan menampilkan pesan “login failed” (pengecekan login hanya mengecek
username dan password, maka dapat multi autentikasi dengan username dan
password yang sama)

★ Pada screen 1 kalian juga dapat menginputkan “register”, setelah menekan
enter anda diminta untuk menginputkan username dan password sama
halnya seperti login

★ Pada register tidak ada pengecekan unique username, maka setelah register
akan langsung menampilkan pesan “register success” dan dapat terjadi
double account

★ Setelah login berhasil maka anda berpindah ke screen 2 dimana
menampilkan 2 fitur seperti berikut.

Screen 2 :
1. Find Match
2. Logout
Choices : {your input}
★ Pada screen 2 anda dapat menginputkan “logout” setelah logout anda akan
kembali ke screen 1

★ Pada screen 2 anda dapat menginputkan “find”, setelah itu akan
menampilkan pesan “Waiting for player ...” print terus sampai menemukan
lawan

★ Jika menemukan lawan maka akan menampilkan pesan “Game dimulai
silahkan ​ tap ​ ​ tap ​ secepat mungkin !!”

★ Pada saat game dimulai diberikan variable health = 100,dan anda dapat
men-​ tap ​ (menekan space pada keyboard tanpa harus menekan enter)

★ Pada saat anda men-​ tap ​ maka akan menampilkan pesan “hit !!”, dan pada
lawan healthnya akan berkurang sebanyak 10 kemudian pada lawan
menampilkan pesan status healthnya sekarang. (conclusion : anda tidak bisa
melihat status health lawan)

★ Jika health anda <= 0 maka akan menampilkan pesan “Game berakhir kamu
kalah”, apabila lawan anda healthnya <= 0 maka akan menampilkan pesan
”Game berakhir kamu menang”

★ Setelah menang atau kalah maka akan kembali ke screen 2

### SERVER SIDE
★ Pada saat program pertama kali dijalankan maka program akan membuat file
akun.txt jika file tersebut tidak ada. File tersebut digunakan untuk menyimpan
username dan password

★ Pada saat user berhasil login maka akan menampilkan pesan “Auth success” jika
gagal “Auth Failed”

★ Pada saat user sukses meregister maka akan menampilkan List account yang
terdaftar (username dan password harus terlihat)

## Jawaban
### Soal 2 belum selesai dikerjakan dikarenakan satu dan lain hal. Berikut adalah penjelasan state program sementara.
Ada 2 file yang dibuat, yaitu file client side dan file server side.

## Client Side
Client side akan menampilkan welcome message, lalu akan meminta user untuk memilih pilihan.
1. Login
2. Register
3. Exit

Jika user mengetikkan login, maka tampilan login akan keluar.
Jika user mengetikkan register, maka tampilan register akan keluar. Ketika user mengetikkan exit, client side dan server side akan melakukan ```exit(EXIT_SUCCESS);```

Ketika memilih login ataupun register, client side akan membuat suatu pointer of file ke file "akun.txt".
Format di dalam file tersebut adalah sebagai berikut :
```
username=some_username;password=some_password;
```

Perbedaan terletak pada jenis ```fopen()``` yang dilakukan login dan register.
Ketika login, ```fopen()``` akan menggunakan mode read. Ketika register, ```fopen()``` akan menggunakan mode append (Karena write akan me-rewrite file lama).
Namun, perintah ini akan dilakukan BUKAN di client side melainkan di server side.

Ketika client side sudah menerima perintah true dari server side (yang menandakan suatu proses berhasil, entah itu login ataupun register), client side akan melanjutkan programnya.
Setelah selesai register, program akan kembali ke menu utama dan pengguna memilih menu login untuk kemudian login.
Saat pengguna berhasil login, menu game akan ditampilkan.
Di dalam menu game, akan ada 2 pilihan, yaitu find dan logout. 
### Error 0 : Menu find belum siap digunakan, stuck di finding match.
### Error 1 : Game belum dapat dimainkan.

Menu logout akan menampilkan pesan terima kasih telah bermain dan akan kembali ke menu login.

## Server Side
Untuk server side, hal yang akan dilakukan untuk pertama kali adalah menunggu input dari user.
Ketika user memilih login ataupun register, fungsi ```makeFile()``` akan berjalan.
```
int makeFile(char username[55], char password[55], int option) // Function to make a file. Option = Login/Register (1/2). Return 1 if true, 0 if false.
{
    int i;
    FILE *fp; // File Pointer
    char temp[512];
    char str[200];

    if(option == 1) // Login -> Read from file
    {
        fp = fopen("akun.txt", "r"); //r -> read

        strcpy(str, "username=");
        strcat(str, username);
        strcat(str, ";password=");
        strcat(str, password);
        strcat(str, ";\n");

        while(fgets(temp, 512, fp) != NULL) // Comparing the data with one in the textfile
        {
            if((strcmp(temp, str)) == 0) // If match found.
            {
                fclose(fp);
                return 1; // True. Match found.
            }
        }
        fclose(fp);

        return 0; // If no match found.
    }
    else if(option == 2) // Register -> Write to file
    {
        fp = fopen("akun.txt", "a"); //w -> write

        strcpy(str, "username=");
        strcat(str, username);
        strcat(str, ";password=");
        strcat(str, password);
        strcat(str, ";\n");

        printf("%s\n", str);

        for (i = 0; str[i] != '\n'; i++)
        {
          /* write to file using fputc() function */
          fputc(str[i], fp);
        }
        fputs("\n",fp);
        fclose(fp);

        return 1;
    }
    
    return 0; // ERROR
}
```

Dalam fungsi inilah ```fopen()``` akan digunakan.

Setelah itu, akan memunculkan message auth successful pada server.
### Error 2 : Belum menampilkan seluruh akun yang ada dalam akun.txt.

Setelah itu, server akan terus berkomunikasi dengan client side untuk menjalankan game.
### Error 3 : Server belum sanggup untuk menerima multiple clients.


# 4. Soal Nomor 4
Link ke file yang dibuat:
* <a href="https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal4/4a.c"><b>4a.c</b></a>
* <a href="https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal4/4b.c"><b>4b.c</b></a>
* <a href="https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal4/4c.c"><b>4c.c</b></a>
# 4.a.
1. Buatlah program C dengan nama "4a.c", yang berisi program untuk
melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan
matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks
nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
2. Tampilkan matriks hasil perkalian tadi ke layar.
# 4.b.
1. Buatlah program C kedua dengan nama "4b.c". Program ini akan
mengambil variabel hasil perkalian matriks dari program "4a.c" (program
sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
(Catatan!: gunakan shared memory)
2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks
tersebut, carilah nilai penjumlahannya dari n sampai 1, dan tampilkan hasilnya ke layar dengan
format seperti matriks.
# 4.c.
1. Buatlah program C ketiga dengan nama "4c.c". Program ini tidak
memiliki hubungan terhadap program yang lalu.
2. Pada program ini, Norland diminta mengetahui jumlah file dan
folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar
IPC, Norland mengerjakannya dengan semangat.
