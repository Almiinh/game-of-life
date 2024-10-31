# Use a lightweight Linux base image
FROM alpine:latest

# Install necessary packages
RUN apk add --no-cache clang make musl-dev ncurses-dev ncurses-static ncurses-terminfo

# Create a working directory
WORKDIR /app

# Copy the source code and Makefile
COPY main.c .
COPY display.c .
COPY Makefile .

# Compile the C program with make
RUN make conway

# Set the entrypoint to run the program
CMD ["./conway"]