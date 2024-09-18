FROM epitechcontent/epitest-docker:latest

RUN apt update

RUN apt install -y clang-tidy bear
