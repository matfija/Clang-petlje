### clang-petlje

problemi:
* pretprocesor → incl, def, comm :x::grey_question:
* kontrola toka → break, continue, goto :x:
* doseg promenljivih → for ima najuži :heavy_check_mark:
* nedostajuće vrednosti → for (;;) :heavy_check_mark:
* ugnežđene petlje za promenu :heavy_check_mark:

predlozi:
* jednoprolazno rešenje → bez glavne petlje
* razdvojiti cpp i hpp → ne sme samo hpp
* natklasa štedi kod → štampaj, zameni...
* pretprocesor → regularni izrazi možda
* kontrola toka → pametna zamena
  * prolazak kroz decu tela
  * for (auto x : s->getBody()->children())
  * zamena svakog deteta ako treba

setup:
```
sudo apt-get update
sudo apt-get install libclang-6.0-dev llvm-6.0-dev clang-format
```

cmake:
```
cd build
cmake ..
make
```

test:
```
./petlje <stari> <novi> [do|while|for]
./petlje hello.c novi.c do
```

tim:
* [jelena jeremić](https://github.com/Jelena597), 62/2016
* [marija erić](https://github.com/MarijaEric), 115/2017
* [lazar vasović](https://github.com/matfija), 99/2016
* [darko nešković](https://github.com/darko123455), 208/2016

