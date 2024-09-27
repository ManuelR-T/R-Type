FROM epitechcontent/epitest-docker:latest

RUN apt update

RUN apt install -y clang-tidy
RUN apt install -y libasio-dev
RUN apt install -y libsfml-dev
RUN apt install -y flex
RUN apt install -y bison
