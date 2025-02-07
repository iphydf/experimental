FROM alpine:3.21

RUN mkdir _site && echo "Hello, World! Here's experimental." >_site/index.html

# Error if $GITHUB_TOKEN is set. We shouldn't see it here.
RUN test -z "$GITHUB_TOKEN"
