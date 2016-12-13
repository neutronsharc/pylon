# Pylon: a performance block storage backend

Pylon aims to become a basic build block for a block storage backend. One instance of 
Pylon is used as one shard for a partitioned block storage.

Pylon is implemented with many features:
- spare file allocation to achieve thin-provision.
- read/write APIs are implemented with mmap mechanism for performance.
- fast snapshot based on filesystem support (reflink of Btrfs).
- built-in journal (todo)
- built-in replication, both from upstream and to downstreams (todo)


