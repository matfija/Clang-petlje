#### clang-petlje

## Transformacija petlji :loop:
Seminarski rad na kursu Konstrukcija kompilatora. Postavljen je sam pisani rad, kao i prateća aplikacija.

Sažetak pisanog rada:
> Razmotrena je upotreba Klanga kao biblioteke u cilju transformacije svih petlji u C kodu u željeni tip (for, while, do-while). Na osnovu uvodnih razmatranja implementirana je aplikacija, u koje svrhe je korišćen Кlangov aplikativni programski interfejs prema apstraktnom sintaksnom stablu. Same izmene vršene su u tekstu koda. Opisano je nekoliko izazova i problema – kako usput rešenih, tako i otvorenih – a neku su ilustrovani pratećim (test) primerima.


## Podešavanje :memo:
Primer podešavanja zavisnosti za Ubuntu:
```
sudo apt-get update
sudo apt-get install libclang-6.0-dev llvm-6.0-dev clang-format
```

Primer prevođenja programa sistemom CMake:
```
cd build
cmake ..
make
```

Primer pokretanja (testiranja) aplikacije:
```
./petlje <ulaz> <izlaz> <do|while|for>
./petlje ulaz.c izlaz.c do
```

## Članovi tima :boy::girl:
* [Lazar Vasović](https://github.com/matfija), 99/2016
* [Jelena Jeremić](https://github.com/Jelena597), 62/2016
