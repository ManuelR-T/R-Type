# Step 1: Install packages in a Fedora-based image
FROM fedora:latest as installer

RUN dnf install -y clang-tools-extra bear

# Step 2: Copy installed packages to the final image
FROM epitechcontent/epitest-docker:latest

COPY --from=installer /usr /usr
COPY --from=installer /usr/bin /usr/bin
