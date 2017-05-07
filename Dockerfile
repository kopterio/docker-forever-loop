FROM scratch

COPY forever-http-loop /

ENTRYPOINT ["/forever-http-loop"]
