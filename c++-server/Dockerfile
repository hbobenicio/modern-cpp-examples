FROM  gcc:13.2.0 as builder
COPY ./src/ /app/
WORKDIR /app/
RUN g++ -std=c++23 -static -Wall -Wextra -Wpedantic -g -O0 -o main main.cpp

FROM scratch
COPY --from=builder /app/main /usr/local/main
CMD ["/usr/local/main"]

