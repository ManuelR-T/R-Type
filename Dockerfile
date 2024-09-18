FROM epitechcontent/epitest-docker:latest

RUN apt update

RUN apt install -y clang-tidy

# RUN apt install -y clang-tools-extra bear
