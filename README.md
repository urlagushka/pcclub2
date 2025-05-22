# pcclub

## Сборка и запуск
#### MacOS (необходим gcc14)
```
-- если не установлен gcc14
brew install gcc

git clone https://github.com/urlagushka/pcclub.git

cd pcclub
mkdir build && cd build

cmake -DCMAKE_C_COMPILER=/opt/homebrew/Cellar/gcc/14.2.0_1/bin/gcc-14 \
      -DCMAKE_CXX_COMPILER=/opt/homebrew/Cellar/gcc/14.2.0_1/bin/g++-14 \
      -DCMAKE_BUILD_TYPE=Release \
      ..

make

-- запуск программы
./pcclub_example <src.txt>
```

#### Linux
```
git clone https://github.com/urlagushka/pcclub.git

cd pcclub
mkdir build && cd build

cmake ..

make

-- запуск программы
./pcclub_example <src.txt>
```

