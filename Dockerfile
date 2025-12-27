# Use Ubuntu as the base image
FROM ubuntu:22.04

# Install build tools and valgrind
RUN apt-get update && \
    apt-get install -y build-essential valgrind && \
    apt-get clean && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /workspace

# Copy the entire project into the container
COPY . /workspace

# Default command: open a shell
CMD ["/bin/bash"]
