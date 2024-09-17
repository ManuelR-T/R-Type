# Use the base image
FROM epitechcontent/epitest-docker:latest

# # Install clang-tidy and bear
RUN sudo dnf install -y clang-tools-extra bear

# Optional: Copy your code into the container if needed
# COPY . .

# Optional: Set a default command
# CMD ["bash"]
