#include <iostream>
#include <fstream>
#include <string>
using namespace std;
struct BITMAPFILEHEADER {
    char bfType[2];  //Sygnatura pliku, ‘BM’ dla prawidłowego pliku BMP 
    unsigned int bfSize; //Długość całego pliku w bajtach
    unsigned short bfReserved1; //Pole zarezerwowane(zwykle zero)
    unsigned short bfReserved2; //Pole zarezerwowane(zwykle zero)
    unsigned int bfOffBits;//Pozycja danych obrazowych w pliku
};
struct BITMAPINFOHEADER {
    unsigned int biSize;  //Rozmiar nagłówka informacyjnego
    unsigned int biWidth;  //Szerokość obrazu w pikselach
    unsigned int biHeight;  //Wysokość obrazu w pikselach
    unsigned short biPlanes;  //Liczba płatów(musi być 1)
    unsigned short biBitCount; //Liczba bitów na piksel : 1, 4, 8, 16, 24 lub 32
    unsigned int biCompression;  //Algorytm kompresji : BI_RGB = 0, BI_RLE8 = 1,BI_RLE4 = 2, BI_BITFIELDS = 3
    unsigned int biSizeImage;  //Rozmiar rysunku
    unsigned int biXpelsPerMeter;  //Rozdzielczość pozioma
    unsigned int biYpelsPerMeter; //Rozdzielczość pionowa
    unsigned int biCrlUses; //Liczba kolorów w palecie
    unsigned int biCrlImportant; //Liczba ważnych kolorów w palecie
};
struct RGB {
    int r;
    int g;
    int b;
    int wolnemiejsce;//wolnemiejsce
};
int detekcjakrawedzi(int pixele[3][3][3], int kolor);
void pustemiejsca(int pixele[3][3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int g = 0; g < 3; g++) {
                pixele[i][j][g] = 0;
            }
        }
    }
}
void odczytywanieplikuBMLP(string filename, BITMAPFILEHEADER &header, BITMAPINFOHEADER &info) {
    fstream plik;
    plik.open(filename, ios::in | ios::binary);
    if (!plik) {
        cout << "Blad" << endl;
    }
    cout << "Podaj nazwe pliku wyjsciowego z dopiskiem.bmp "<<endl;
    string nazwa;
    cin >> nazwa;
    ofstream newbmp(nazwa, ios::binary);
   
    //ODCZYTYWANIE BITMAPFILEHEADER
    plik.read(reinterpret_cast<char*>(&header.bfType[0]), 1);
    plik.read(reinterpret_cast<char*>(&header.bfType[1]), 1);
    if (header.bfType[0] != 'B' and header.bfType[1] != 'P') {
        cout << "blad!";
        exit(1);
    }
    plik.read(reinterpret_cast<char*>(&header.bfSize), 4);
    plik.read(reinterpret_cast<char*>(&header.bfReserved1), 2);
    plik.read(reinterpret_cast<char*>(&header.bfReserved2), 2);
    plik.read(reinterpret_cast<char*>(&header.bfOffBits), 4);
    //ODCZYTYWANIE BITMAPINFOHEADER
    plik.read(reinterpret_cast<char*>(&info.biSize), 4);
    plik.read(reinterpret_cast<char*>(&info.biWidth), 4);
    plik.read(reinterpret_cast<char*>(&info.biHeight), 4);
    plik.read(reinterpret_cast<char*>(&info.biPlanes), 2);
    plik.read(reinterpret_cast<char*>(&info.biBitCount), 2);
    plik.read(reinterpret_cast<char*>(&info.biCompression), 4);
    plik.read(reinterpret_cast<char*>(&info.biSizeImage), 4);
    plik.read(reinterpret_cast<char*>(&info.biXpelsPerMeter), 4);
    plik.read(reinterpret_cast<char*>(&info.biYpelsPerMeter), 4);
    plik.read(reinterpret_cast<char*>(&info.biCrlUses), 4);
    plik.read(reinterpret_cast<char*>(&info.biCrlImportant), 4);

    //zapis BITMAPFILEHEADER
    newbmp.write(reinterpret_cast<char*>(&header.bfType[0]), 1);
    newbmp.write(reinterpret_cast<char*>(&header.bfType[1]), 1);
    newbmp.write(reinterpret_cast<char*>(&header.bfSize), 4);
    newbmp.write(reinterpret_cast<char*>(&header.bfReserved1), 2);
    newbmp.write(reinterpret_cast<char*>(&header.bfReserved2), 2);
    newbmp.write(reinterpret_cast<char*>(&header.bfOffBits), 4);
    //zapis BITMAPINFOHEADER
    newbmp.write(reinterpret_cast<char*>(&info.biSize), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biWidth), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biHeight), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biPlanes), 2);
    newbmp.write(reinterpret_cast<char*>(&info.biBitCount), 2);
    newbmp.write(reinterpret_cast<char*>(&info.biCompression), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biSizeImage), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biXpelsPerMeter), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biYpelsPerMeter), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biCrlUses), 4);
    newbmp.write(reinterpret_cast<char*>(&info.biCrlImportant), 4);

    RGB* rgb = new RGB[info.biCrlImportant];//utworzenie tabicy struktur dla palety barw o rozmiarze liczby kolorow
    for (int i = 0; i < info.biCrlImportant; i++) {
        plik.read(reinterpret_cast<char*>(&rgb[i].b), 1);
        plik.read(reinterpret_cast<char*>(&rgb[i].g), 1);
        plik.read(reinterpret_cast<char*>(&rgb[i].r), 1);
        plik.read(reinterpret_cast<char*>(&rgb[i].wolnemiejsce), 1);

        newbmp.write(reinterpret_cast<char*>(&rgb[i].b), 1);
        newbmp.write(reinterpret_cast<char*>(&rgb[i].g), 1);
        newbmp.write(reinterpret_cast<char*>(&rgb[i].r), 1);
        newbmp.write(reinterpret_cast<char*>(&rgb[i].wolnemiejsce), 1);
    }
    char bufor;
    for (int i = plik.tellg(); i < header.bfOffBits; i++) {
        plik.read(reinterpret_cast<char*>(&bufor), 1);
        newbmp.write(reinterpret_cast<char*>(&bufor), 1);
    }
    int*** bitmapa;
    bitmapa = new int** [3];//tworzenie pierwszego wymiaru
    int*** outbitmapa;
    outbitmapa = new int** [3];//tworzenie pierwszego wymiaru

    for (int i = 0; i < 3; i++) {


        bitmapa[i] = new int* [info.biHeight];//tworzenie drugiego wymiaru
        outbitmapa[i] = new int* [info.biHeight];

        for (int j = 0; j < info.biHeight; j++) {

            bitmapa[i][j] = new int[info.biWidth];//tworzenie trzeciego wymiaru
            outbitmapa[i][j] = new int[info.biWidth];
        }
    }
    //ustawianiepixeliujemnych
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < info.biHeight; j++) {
            for (int g = 0; g < info.biWidth; g++) {
                bitmapa[i][j][g] = -1;
            }
        }
    }
    int iloscpxl = (((header.bfSize - header.bfOffBits) - info.biWidth * info.biHeight * info.biBitCount / 8) / info.biHeight);
    int pixele[3][3][3] = {};
    pustemiejsca(pixele);
    plik.seekg(header.bfOffBits, ios::beg);// ustawienie miejsca do oczytu plikow na pozycje przed dane obrazowe
    for (int i = 0; i < info.biHeight; i++) {
        for (int j = 0; j < info.biWidth; j++) {
            for (int kolor = 0; kolor < 3; kolor++) {
                plik.read(reinterpret_cast<char*>(&bitmapa[kolor][i][j]), info.biBitCount / 8);
            }
            if (iloscpxl > 0) {
                for (int x = 0; x <= (((header.bfSize - header.bfOffBits) - info.biWidth * info.biHeight * info.biBitCount / 8)) / info.biHeight; x++) {
                    if (iloscpxl > 0) {
                        plik.ignore(info.biBitCount / 8);
                        iloscpxl--;
                    }
                }
            }
        }
    }
    int puste = 0;
    int kolor = 0;
    int mnoznik = 9;
    //detekcja krawdzi wywolanie
    //kolor b
    for (int i = 0; i < info.biHeight; i++) {
        for (int j = 0; j < info.biWidth; j++) {
            kolor = 0;
            for (int x = 0; x < 3; x++) {
                for (int z = 0; z < 3; z++) {
                    if ((i - 1 + x) < 0 || (i - 1 + x > info.biHeight - 1)) pixele[kolor][x][z] = 255;
                    if ((j - 1 + z) < 0 || (j - 1 + z > info.biWidth - 1)) pixele[kolor][x][z] = 255;
                    else if ((i - 1 + x) >= 0 && (i - 1 + x <= info.biHeight - 1)) pixele[kolor][x][z] = bitmapa[kolor][i - 1 + x][j - 1 + z];
                }
            }
            if (bitmapa[kolor][i][j] != -1)outbitmapa[kolor][i][j] = detekcjakrawedzi(pixele, kolor);
        }
    }
    //kolor g
    for (int i = 1; i < info.biHeight; i++) {
        for (int j = 1; j < info.biWidth; j++) {
            kolor = 1;
            for (int x = 0; x < 3; x++) {
                for (int z = 0; z < 3; z++) {
                    if ((i - 1 + x) < 0 || (i - 1 + x > info.biHeight - 1)) pixele[kolor][x][z] = 255;
                    if ((j - 1 + z) < 0 || (j - 1 + z > info.biWidth - 1)) pixele[kolor][x][z] = 255;
                    else if ((i - 1 + x) >= 0 && (i - 1 + x <= info.biHeight - 1)) pixele[kolor][x][z] = bitmapa[kolor][i - 1 + x][j - 1 + z];
                }
            }
            if (bitmapa[kolor][i][j] != -1)outbitmapa[kolor][i][j] = detekcjakrawedzi(pixele, kolor);
        }
    }
    //kolor r
    for (int i = 2; i < info.biHeight; i++) {
        for (int j = 2; j < info.biWidth; j++) {
            kolor = 2;
            for (int x = 0; x < 3; x++) {
                for (int z = 0; z < 3; z++) {
                    if ((i - 1 + x) < 0 || (i - 1 + x > info.biHeight - 1)) pixele[kolor][x][z] = 255;
                    if ((j - 1 + z) < 0 || (j - 1 + z > info.biWidth - 1)) pixele[kolor][x][z] = 255;
                    else if ((i - 1 + x) >= 0 && (i - 1 + x <= info.biHeight - 1)) pixele[kolor][x][z] = bitmapa[kolor][i - 1 + x][j - 1 + z];
                }
            }
            if (bitmapa[kolor][i][j] != -1)outbitmapa[kolor][i][j] = detekcjakrawedzi(pixele, kolor);
        }
    }
    for (int i = 0; i < info.biHeight; i++) {
        for (int j = 0; j < info.biWidth; j++) {
            for (int kolor = 0; kolor < 3; kolor++) {
                newbmp.write(reinterpret_cast<char*>(&outbitmapa[kolor][i][j]), (info.biBitCount / 8));
            }
            if (iloscpxl > 0) {
                for (int x = 0; x <= (((header.bfSize - header.bfOffBits) - info.biWidth * info.biHeight * info.biBitCount / 8)) / info.biHeight; x++)
                {
                    if (iloscpxl > 0) {
                        plik.write(reinterpret_cast<char*>(&puste), info.biBitCount / 8);
                        iloscpxl--;
                    }
                }
            }
        }
    }
    //zwalnianie pamieci
    for (int i = 0; i < 3; i++) //zwalnianie pamieci
    {
        for (int j = 0; j < info.biHeight; j++)
        {
            delete[] bitmapa[i][j]; //zwalnianie trzeciego wymiaru
            delete[] outbitmapa[i][j];
        }
        delete[] bitmapa[i]; //zwalnianie drugiego wymiaru
        delete[] outbitmapa[i];
    }
    delete[] bitmapa; //zwalnianie pierwszego wymiaru
    delete[] outbitmapa;
    //delete[] rgb;
    plik.close();
    newbmp.close();
}
void wypisywanie(BITMAPFILEHEADER &header, BITMAPINFOHEADER &info) {
    cout << header.bfType[0] << header.bfType[1] <<" :Sygnatura pliku, 'BM' dla prawidlowego pliku BMP" << endl;
    cout << header.bfSize << " :Dlugosc calego pliku w bajtach" << endl;
    cout << header.bfReserved1 << " :Pole zarezerwowane(zwykle zero)" << endl;
    cout << header.bfReserved2 << " :Pole zarezerwowane(zwykle zero)" << endl;
    cout << header.bfOffBits << " :Pozycja danych obrazowych w pliku" << endl;
    cout << info.biSize << " :Rozmiar naglowka informacyjnego" << endl;
    cout << info.biWidth << " :Szerokosc obrazu w pikselach" << endl;
    cout << info.biHeight << " :Wysokosc obrazu w pikselach" << endl;
    cout << info.biPlanes << " :Liczba platow(musi byc 1)" << endl;
    cout << info.biBitCount << " :Liczba bitow na piksel : 1, 4, 8, 16, 24 lub 32" << endl;
    cout << info.biCompression << " :Algorytm kompresji : BI_RGB = 0, BI_RLE8 = 1,BI_RLE4 = 2, BI_BITFIELDS = 3" << endl;
    cout << info.biSizeImage << " :Rozmiar rysunku" << endl;
    cout << info.biXpelsPerMeter << " :Rozdzielczosc pozioma" << endl;
    cout << info.biYpelsPerMeter << " :Rozdzielczosc pionowa" << endl;
    cout << info.biCrlUses << " :Liczba kolorow w palecie" << endl;
    cout << info.biCrlImportant << " :Liczba waznych kolorow w palecie" << endl;
}
void maskisobel(){
    short s_0[3][3] = {{ 1, 2, 1},{ 0, 0, 0},{-1, -2, -1} };
    short s_1[3][3] = {{ 2, 1, 0},{ 1, 0, -1},{ 0, -1, -2} };
    short s_2[3][3] = {{ 1, 0, -1},{ 2, 0, -2},{ 1, 0, -1} };
    short s_3[3][3] = {{ 0, -1, -2},{ 1, 0, -1},{ 2, 1, 0} };
    short s_4[3][3] = {{-1, -2, -1},{ 0, 0, 0},{ 1, 2, 1} };
    short s_5[3][3] = {{-2, -1, 0},{-1, 0, 1},{ 0, 1, 2} };
    short s_6[3][3] = {{-1, 0, 1},{-2, 0, 2},{-1, 0, 1} };
    short s_7[3][3] = {{ 0, 1, 2},{-1, 0, 1},{-2, -1, 0} };
}
int detekcjakrawedzi(int pixele[3][3][3], int kolor) {
    int wyjscieodej = 0, maxwyj = 0;
    wyjscieodej = abs(pixele[kolor][0][0] - pixele[kolor][2][2]);
    if (wyjscieodej > maxwyj)maxwyj = wyjscieodej;
    wyjscieodej = abs(pixele[kolor][0][1] - pixele[kolor][2][1]);
    if (wyjscieodej > maxwyj)maxwyj = wyjscieodej;
    wyjscieodej = abs(pixele[kolor][1][0] - pixele[kolor][1][2]);
    if (wyjscieodej > maxwyj)maxwyj = wyjscieodej;
    wyjscieodej = abs(pixele[kolor][0][2] - pixele[kolor][2][0]);
    if (wyjscieodej > maxwyj)maxwyj = wyjscieodej;
    return maxwyj;
}

int main(){
    string nazwa;
    BITMAPFILEHEADER fileheader{};
    BITMAPINFOHEADER infoheader{};
    RGB rgb{};
    cout << "Podaj nazwe pliku z dopskiem.bmp" << endl;
    while (cin >> nazwa) {
        if (nazwa[nazwa.size() - 1] == 'p')break;
        cout << "Podaj nazwe pliku z dopskiem.bmp" << endl;
    }
    odczytywanieplikuBMLP(nazwa, fileheader, infoheader);
    wypisywanie(fileheader, infoheader);
    return 0;
}