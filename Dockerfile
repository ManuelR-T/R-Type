FROM epitechcontent/epitest-docker:latest

RUN apt update

RUN apt install -y clang-tidy
RUN apt install -y flex
RUN apt install -y bison
RUN apt install -y libflac-dev
