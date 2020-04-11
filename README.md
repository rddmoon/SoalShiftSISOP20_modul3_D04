# Laporan Penjelasan dan Penyelesaian Praktikum Sistem Operasi 2020
# Modul 3
## Kelompok D04
1. Michael Ricky (05111840000078)
2. Yaniar Pradityas Effendi (05111840000047)
# 2. Soal Nomor 2
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

# 3. Soal Nomor 3
## Soal
Buatlah sebuah program dari C untuk mengkategorikan file. Program ini akan
memindahkan file sesuai ekstensinya (tidak case sensitive. JPG dan jpg adalah
sama) ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working
directory ketika program kategori tersebut dijalankan.
● Pada opsi -f tersebut, user bisa menambahkan argumen file yang bisa
dikategorikan sebanyak yang user inginkan seperti contoh di atas.
● Pada program kategori tersebut, folder jpg,c,zip tidak dibuat secara manual,
melainkan melalui program c. Semisal ada file yang tidak memiliki ekstensi,
maka dia akan disimpan dalam folder “Unknown”.
● Program kategori ini juga menerima perintah (*) seperti di bawah;
● Artinya mengkategori seluruh file yang ada di working directory ketika
menjalankan program C tersebut.
● Selain hal itu program C ini juga menerima opsi -d untuk melakukan kategori
pada suatu directory. Untuk opsi -d ini, user hanya bisa menginput 1 directory
saja, tidak seperti file yang bebas menginput file sebanyak mungkin.
● Hasilnya perintah di atas adalah mengkategorikan file di /path/to/directory dan
hasilnya akan disimpan di working directory di mana program C tersebut
berjalan (hasil kategori filenya bukan di /path/to/directory).
● Program ini tidak rekursif. Semisal di directory yang mau dikategorikan, atau
menggunakan (*) terdapat folder yang berisi file, maka file dalam folder
tersebut tidak dihiraukan, cukup file pada 1 level saja.
● Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan
secara paralel sehingga proses kategori bisa berjalan lebih cepat. Dilarang
juga menggunakan fork-exec dan system.
● Silahkan download soal3.zip sebagai percobaan. Namun silahkan
dicoba-coba sendiri untuk kemungkinan test case lainnya yang mungkin
belum ada di soal3.zip.

## Penjelasan
Class Struct.
```
struct name{
  char path[500];
  char file[100];
};
```
Fungsi untuk mengambil extention dari file.
```
char *getext(char *name) {
  char *ext = strrchr(name, '.');
  if(ext == NULL){
    return "";
  }
  return ext + 1;
}
```
Fungsi untuk mengambil nama file yang diinput pada opsi -f.
```
char *getname(char *path) {
  char *name = strrchr(path, '/');
  if(name == NULL){
    return "";
  }
  return name++;
}
```
Fungsi lowercase merupakan fungsi untuk mengubah extention menjadi file.
```
void lowercase(char *ext){
  int i;
  for (i = 0; i < strlen(ext); i++){
      ext[i] = tolower(ext[i]);
  }
}
```
Fungsi moving fungsi untuk memindahkan file ke folder sesuai extention-nya
```
void* moving(void* param){
  struct name *n = param;
  char file1[700];
  char file2[700];
  char *ext = getext(n->file);
  lowercase(ext);
  if(ext == ""){
    mkdir("Unknown", 0777);
    sprintf(file2, "./Unknown/%s", n->file);
  }
  else{
    mkdir(ext, 0777);
    sprintf(file2, "./%s/%s", ext, n->file);
  }
  //sprintf(file1, "%s/%s", n->path, n->file);
  strcpy(file1, n->path);
  rename(file1, file2);
  free(n);
}
```
Fungsi untuk mengecek input. Apakah user memilih opsi -d, -f, atau ```*```, atau input salah.
```
if(argc == 2){
    if(strcmp(argv[1], "*") == 0){
      strcpy(drct, ".");
      if((dir = opendir(drct)) == NULL){
        printf("ERROR: gagal mengakses directory\n");
        exit(EXIT_FAILURE);
      }
    }
    else{
      printf("ERROR: argumen yang diberikan tidak sesuai\n");
      exit(EXIT_FAILURE);
    }
  }
  else if(argc > 2){
    if(argc == 3 && strcmp(argv[1], "-d") == 0){
      strcpy(drct, argv[2]);
      if((dir = opendir(drct)) == NULL){
        printf("ERROR: gagal mengakses directory\n");
        exit(EXIT_FAILURE);
      }
    }
    else if(strcmp(argv[1], "-f") == 0){
      pthread_t threads[argc];
      for(i = 2; i < argc; i++){
        char *n = getname(argv[i]);
        if(n == ""){
          printf("ERROR: argumen yang diberikan tidak sesuai\n");
          exit(EXIT_FAILURE);
        }
        struct name *f = (struct name *)malloc(sizeof(struct name));
        strcpy(f->file, n);
        strcpy(f->path, argv[i]);
        pthread_create(&threads[i], NULL, moving, (void *)f);
      }
      for(i = 2; i < argc; i++){
          pthread_join(threads[i], NULL);
      }
      exit(EXIT_SUCCESS);
    }
    else{
      printf("ERROR: argumen yang diberikan tidak sesuai\n");
      exit(EXIT_FAILURE);
    }
  }
  else{
    printf("ERROR: argumen yang diberikan tidak sesuai\n");
    exit(EXIT_FAILURE);
  }
```
Pada bagian di bawah di fungsi main, untuk membuat pthread dan memanggil fungsi moving untuk memindah tiap file.
```
int count = 0;
  int cnt = 0;
  while((d = readdir(dir)) != NULL){
    if(d->d_type == DT_REG){
      count++;
    }
  }
  rewinddir(dir);
  pthread_t threads[count];
  while((d = readdir(dir)) != NULL){
    if(d->d_type != DT_REG){
      continue;
    }
    if(strcmp(argv[0]+2, d->d_name) == 0){
      threads[cnt] = 0;
      cnt++;
      continue;
    }
    struct name *f = (struct name *)malloc(sizeof(struct name));
    strcpy(f->file, d->d_name);
    sprintf(f->path, "%s/%s", drct, f->file);
    //strcpy(f->path, drct);
    pthread_create(&threads[cnt], NULL, moving, (void *)f);
    cnt++;
  }
  for(i = 0; i < count; i++){
    if(threads[i]){
      pthread_join(threads[i], NULL);
    }
  }
  exit(EXIT_SUCCESS);
```

# 4. Soal Nomor 4
Link ke file yang dibuat:
* <a href="https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal4/4a.c">4a.c</a> - Soal 4a
* <a href="https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal4/4b.c">4b.c</a> - Soal 4b
* <a href="https://github.com/rddmoon/SoalShiftSISOP20_modul3_D04/blob/master/soal4/4c.c">4c.c</a> - Soal 4c
# 4.a.
1. Buatlah program C dengan nama "4a.c", yang berisi program untuk
melakukan perkalian matriks. Ukuran matriks pertama adalah 4x2, dan
matriks kedua 2x5. Isi dari matriks didefinisikan di dalam kodingan. Matriks
nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).
2. Tampilkan matriks hasil perkalian tadi ke layar.
## Jawaban
```
int a[B][BK] = {{2,4},{1,2},{3,5},{6,1}};
int b[BK][K] = {{3,2,3,2,1},{1,1,2,1,3}};
int c[B][K];
```
definisi matriks a, b, dan matriks c untuk hasil perkaliannya
```
void *kali(void *param) {
   struct m *mtrx = param;
   int k, sum = 0;

   for(k = 0; k < BK; k++){
      sum += a[mtrx->i][k] * b[k][mtrx->j];
   }
   c[mtrx->i][mtrx->j] = sum;

   pthread_exit(0);
}
```
fungsi untuk menghitung perkalian matriks ab dan b, kemudian menyimpan hasil untuk tiap elemen matriks c
```
key_t key = ftok("/home/yaniarpe/modul3",'a');
```
Bagian di atas untuk men-generate unique id
```
int shmid = shmget(key, sizeof(int)*B*K, IPC_CREAT|0666);
int* value = (int *)shmat(shmid, (void*)0, 0);
```
untuk mendapatkan id segment shared memory dan attach ke shared memory
```
for(i = 0; i < B; i++) {
      for(j = 0; j < K; j++) {

         struct m *mtrx = (struct m *) malloc(sizeof(struct m));
         mtrx->i = i;
         mtrx->j = j;
		 pthread_t tid; 
         pthread_attr_t attr; 
         pthread_attr_init(&attr);
         pthread_create(&tid,&attr,kali,mtrx);
         pthread_join(tid, NULL);
      }
   }
```
Bagian ini untuk menghitung perkalian matriks tiap elemen menggunakan thread
```
printf("Matriks hasil perkalian:\n");
   for(i = 0; i < B; i++) {
      for(j = 0; j < K; j++) {
         printf("%d\t", c[i][j]);
         value[i*K+j] = c[i][j];
      }
      printf("\n");
   }
   shmdt(value);

```
Setelah selesai menghitung maka matriks c hasil perkalian dimunculkan ke layar dan nilai matriks c tersebut disimpan di shared memory. Setelah selesai semua maka lepaskan shared memory, jangan di destroy.

# 4.b.
1. Buatlah program C kedua dengan nama "4b.c". Program ini akan
mengambil variabel hasil perkalian matriks dari program "4a.c" (program
sebelumnya), dan tampilkan hasil matriks tersebut ke layar.
(Catatan!: gunakan shared memory)
2. Setelah ditampilkan, berikutnya untuk setiap angka dari matriks
tersebut, carilah nilai penjumlahannya dari n sampai 1, dan tampilkan hasilnya ke layar dengan
format seperti matriks.
## Jawaban
```
int tambah(int x){
  int i;
  int result = 1;
  if(x == 1){
    return 1;
  }
  else{
    for(i = 1; i <= x; i++){
      result += i;
    }
    return result;
  }
}
```
Fungsi di atas untuk menghitung penjumlahan dari 1 sampai n.
```
void *calc(void *param) {
  struct m *mtrx = param;
   int result;
   result = tambah(matrix[mtrx->i][mtrx->j]);
   f[mtrx->i][mtrx->j] = result;
   //Exit thread
   pthread_exit(0);
}
```
Fungsi di atas untuk memanggil fungsi tambah untuk menghitung penjumlahan dan menyimpan hasilnya ke dalam matriks f.
```
void main(){

  key_t key = ftok("/home/yaniarpe/modul3",'a');
  int i, j;

  int shmid = shmget(key, sizeof(int)*B*K, IPC_CREAT|0666);
  int* value = (int *)shmat(shmid, (void*)0, 0);
  printf("Hasil perkalian matriks di 4a:\n");
  for(i = 0; i < B; i++) {
    for(j = 0; j < K; j++) {
      printf("%d\t", value[i*K+j]);
      matrix[i][j] = value[i*K+j];
    }
    printf("\n");
  }
  for(i = 0; i < B; i++) {
     for(j = 0; j < K; j++) {
        
        struct m *mtrx = (struct m *) malloc(sizeof(struct m));
        mtrx->i = i;
        mtrx->j = j;
        pthread_t tid;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tid,&attr,calc,mtrx);
        pthread_join(tid, NULL);
     }
  }
  printf("Penjumlahan elemen matriks:\n");
  for(i = 0; i < B; i++){
    for(j = 0; j < K; j++){
      printf("%d\t", f[i][j]);
    }
    printf("\n");
  }
  shmdt(value);
  shmctl(shmid, IPC_RMID, NULL);
}
```
Pada fungsi main mengambil data dari array yang berada di shared memory, kemudian melakukan operasi penjumlahan dari n sampai 1 untuk tiap elemen matriks menggunakan thread, lalu menampilkan hasilnya. Terakhir, detach dari shared memory dan destroy shared memory karena sudah tidak dipakai.

# 4.c.
1. Buatlah program C ketiga dengan nama "4c.c". Program ini tidak
memiliki hubungan terhadap program yang lalu.
2. Pada program ini, Norland diminta mengetahui jumlah file dan
folder di direktori saat ini dengan command "ls | wc -l". Karena sudah belajar
IPC, Norland mengerjakannya dengan semangat.
## Jawaban
```
int fd[2];
  pipe(fd);
```
Membuat sebuah pipe.
```
child_id = fork();
  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }
  if (child_id == 0){
    //this is child
    close(fd[0]);
    dup2(fd[1], 1); //stdout write pipe
    close(fd[1]);
    execlp("ls","ls", NULL);
  }
  ```
  Merupakan child pertama. Untuk menjalanakn ls dengan execlp. Di child pertama stdout berperan untuk melakukan write pada pipe, sehingga fd[1] digunakan dan untuk fd[0] (read) lebih baik ditutup karena tidak dipakai. Ujung pipe read seharusnya berada pada program yang menjalankan wc -l, terhubung pada stdin untuk membaca data pada pipe.
  ```
  child_id2 = fork();
  if (child_id2 < 0) {
    exit(EXIT_FAILURE);
  }
  if (child_id2 == 0){
    //this is child
    close(fd[1]);
    dup2(fd[0], 0); //stdin read pipe
    close(fd[0]);
    execlp("wc","wc","-l", NULL);
  }
  ```
  Di child kedua menjalankan wc -l dengan execlp. Seperti yang telah dijelaskan sebelumnya, stdin berfungsi sebagai read pada ujung pipe, sehingga fd[1] (write) tidak digunakan dan sebaiknya ditutup.
  ```
  //this is parent
  close(fd[0]);
  close(fd[1]);
  int i;
  for(i = 0; i < 2; i++){
    wait(NULL);
  }
  ```
  Pada parent cukup melakukan forking untuk membuat 2 child dan melakukan wait() untuk semua proses child yang ada hingga selesai.
