# Stage 1: Install clang-tidy and bear using Fedora
FROM fedora:latest as builder

# Install clang-tidy and bear
RUN dnf install -y clang-tools-extra bear

# Stage 2: Use the base image for the final build
FROM epitechcontent/epitest-docker:latest
