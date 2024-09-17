# Stage 1: Build Stage
FROM fedora:latest as builder

# Install necessary build tools in the builder stage
RUN dnf install -y clang-tools-extra bear

# Stage 2: Final Stage
FROM epitechcontent/epitest-docker:latest

# Copy necessary files or artifacts from the builder stage
# COPY --from=builder /path/to/artifacts /path/to/destination

# If `dnf` is needed in the final stage, install it using the appropriate package manager.
# If the final stage is based on Debian/Ubuntu, use `apt` instead of `dnf`.
# RUN apt-get update && apt-get install -y clang-tools-extra bear

# Your additional commands for the final image
