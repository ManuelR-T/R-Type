FROM fedora:latest as builder

RUN dnf install -y clang-tools-extra bear

FROM epitechcontent/epitest-docker:latest

COPY --from=builder /usr/bin/clang-tidy /usr/bin/clang-tidy
COPY --from=builder /usr/lib64/clang /usr/lib64/clang
