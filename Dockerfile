# Stage 1: Build tools in Fedora
FROM fedora:latest as builder
RUN dnf install -y clang-tools-extra bear

# Stage 2: Use the epitechcontent/epitest-docker image
FROM epitechcontent/epitest-docker:latest

# Copy clang-tidy and bear
COPY --from=builder /usr/bin/clang-tidy /usr/bin/clang-tidy
COPY --from=builder /usr/bin/bear /usr/bin/bear

# Copy necessary libraries (adjust the path as needed)
COPY --from=builder /usr/lib64/clang /usr/lib64/clang
COPY --from=builder /usr/lib64/libclang-cpp.so* /usr/lib64/
