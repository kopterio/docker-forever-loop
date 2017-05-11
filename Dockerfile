# --- builder container ---

FROM alpine:3.4

RUN apk update
RUN apk add gcc musl-dev

COPY forever-http-loop.c /
RUN gcc -Wall -o forever-http-loop forever-http-loop.c -static
RUN strip forever-http-loop

# --- actual docker image recipe ---

FROM scratch

COPY --from=0 /forever-http-loop /

ENTRYPOINT ["/forever-http-loop"]
