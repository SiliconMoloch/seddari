# [Seddari](https://fr.wiktionary.org/wiki/seddari)

A small multi-client TCP chat server written in C. It uses non-blocking sockets and `select()` to handle several clients from a single event loop.

## Features

- TCP server with configurable IPv4 address and port.
- Non-blocking server and client sockets.
- Multiple simultaneous clients through `select()`.
- Line-based text protocol.
- Broadcast messages to connected clients.
- Client commands:
  - `/help` — display available commands.
  - `/nick <name>` — change nickname.
  - `/list` — list connected clients.
  - `/quit` — disconnect from the server.
- Per-client message rate limit.
- Dynamic output buffers with a maximum size.
- Graceful shutdown on `SIGINT` and `SIGTERM`.
- Protection against `SIGPIPE` when a client disconnects unexpectedly.

## Requirements

- A POSIX-compatible operating system.
- A C compiler such as `cc` or `clang`.
- Standard POSIX socket APIs.

## Build

Use the build command provided by the project:

```sh
make
```

## Usage

Start the server with the default configuration:

```sh
./seddari
```

The default address is `127.0.0.1` and the default port is `8080`.

You can specify another address and port:

```sh
./server --address 127.0.0.1 --port 8080
```

Short options are also available:

```sh
./server -a 127.0.0.1 -p 8080
```

Display the help message:

```sh
./server --help
```

## Connecting

You can connect with a command-line TCP client such as `nc`:

```sh
nc 127.0.0.1 8080
```

Open a second terminal and connect again to test broadcasting between clients.

Each message must be terminated with a newline. For example:

```text
Hello everyone!
/nick Lou
/list
/help
/quit
```

## Protocol

The server uses a simple line-based text protocol. A received line is either interpreted as a command or broadcast as a chat message.

Messages are limited by the server's receive buffer. The server may disconnect clients that send invalid, oversized, or excessively frequent input.

## Rate limiting

The server limits the number of messages processed by each client during a one-second window. The current limit is defined by `MAX_MSG_PER_SEC` in `server.h`.

The implementation uses `time_t`, so the rate-limit window has a resolution of one second. This is intentional for the current implementation; a future version could use a monotonic high-resolution clock for finer-grained limits.

## Design overview

The main components are separated by responsibility:

- `t_server` stores listening-socket state, `select()` sets, configuration, and the linked list of clients.
- `t_client` stores one client's socket, buffers, nickname, and rate-limit counters.
- The accept logic creates and registers new clients.
- The read logic receives data and accumulates complete lines.
- The command logic handles slash commands.
- The write logic flushes pending output without blocking.
- The cleanup logic removes clients and releases allocated memory.

## Known limitations

This is a learning project and is not intended to be a production-ready public service yet.

- The server currently uses `select()`, which is suitable for a small number of clients but does not scale as well as `poll()`, `epoll()`, or `kqueue()`.
- Connection attempts are not yet rate-limited per IP address.
- There is no idle-client timeout.
- The nickname and protocol validation can be made stricter.
- The rate limiter uses one-second resolution.
- The server should be tested with sanitizers and stress tests before deployment.

## Testing checklist

Before publishing, verify at least:

- The project builds with warnings enabled.
- Messages are broadcast correctly.
- `/nick`, `/list`, `/help`, and `/quit` work.
- A client disconnecting abruptly does not terminate the server.
- Oversized input is rejected safely.
- Repeated messages trigger the rate limit without crashing the server.
- `Ctrl-C` shuts down the server and releases resources.

Useful local tools include `nc`, `valgrind`, and compiler sanitizers such as AddressSanitizer.

## Project status

Functional learning project. The server is suitable for local experimentation and as a foundation for further network-programming work. Any suggestion is welcome.

## License

No license has been selected yet. Add a license before accepting external contributions or redistributing the project.