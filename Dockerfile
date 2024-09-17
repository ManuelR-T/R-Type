# Stage 1: Build tools in Fedora
FROM fedora:latest as builder
RUN dnf install -y clang-tools-extra bear

# Stage 2: Use the epitechcontent/epitest-docker image
FROM epitechcontent/epitest-docker:latest

COPY --from=builder /usr/bin/clangd /usr/bin/clang-tidy
COPY --from=builder /usr/bin/bear /usr/bin/bear
