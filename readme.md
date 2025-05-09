# NS-3 Differentiated Services (DiffServ) QoS Project

This project implements **Strict Priority Queueing (SPQ)** and **Deficit Round Robin (DRR)** mechanisms in the [ns-3 network simulator](https://www.nsnam.org/)(ns-3.44), using a common base class `DiffServ` to abstract logic of different differentiated services. It includes both simulation implementations and test topologies.

## Project Structure & Build Guidance
Have ns-3.44 library installed in your Ubuntu Virtual machine. In the root ns-3 directory, create a project folder under `scratch` folder:
```
mkdir scratch/NS3-DifferentiatedServices
```
Place **all source files and configuration files** inside the `scratch/NS3-DifferentiatedServices/` directory of your `ns-3-dev` workspace. The project includes:

- `diff-serv.cc`, `diff-serv.h`: Base class for DiffServ behaviors
- `traffic-class.cc`, `traffic-class.h`: Per-class queue configuration
- `filter.cc`, `filter.h`, `filter-element.cc`, `filter-element.h`: Packet classification filter module
- `spq.cc`, `spq.h`: Implementation of SPQ
- `drr-queue.cc`, `drr-queue.h`: Implementation of DRR
- `main-spq-simulation.cc`: SPQ simulation runner
- `main-drr-simulation.cc`: DRR simulation runner
- `qos-initializer.cc`, `qos-initializer.h`: used to initialize `DiffServ` class in object factory design pattern
- `json.hpp`: nlohmann json library file used to parse json configurations
- `spq.json`, `drr.json`: Queue configuration files for simple filtering senarios
- `spq-complex-filters.json` / `drr-complex-filters.json`: Queue configuration files to test every filter element and complex senarios

Due to ns-3's limitation of supporting only **one `main()` function** at a time in the `scratch` folder, **rename the unused `main-*.cc` to `*.cc.bak`** before running the desired simulation.

### Run SPQ Simulation

```bash

# Rename DRR simulation file to disable it
mv scratch/NS3-DifferentiatedServices/main-drr-simulation.cc scratch/NS3-DifferentiatedServices/main-drr-simulation.cc.bak
mv scratch/NS3-DifferentiatedServices/main-spq-simulation.cc.bak scratch/NS3-DifferentiatedServices/main-spq-simulation.cc

# Run SPQ simulation
./ns3 run scratch/NS3-DifferentiatedServices/main-spq-simulation --command-template="%s --spqConfig=/path/to/your/spq.json"
```

### Run DRR Simulation

```bash
# Rename SPQ simulation file to disable it
mv scratch/NS3-DifferentiatedServices/main-spq-simulation.cc scratch/NS3-DifferentiatedServices/main-spq-simulation.cc.bak
mv scratch/NS3-DifferentiatedServices/main-drr-simulation.cc.bak scratch/NS3-DifferentiatedServices/main-drr-simulation.cc

# Run DRR simulation
./ns3 run scratch/NS3-DifferentiatedServices/main-drr-simulation --command-template="%s --drrConfig=/path/to/your/drr.json"

```



##  Implemented QoS Mechanisms

###  Strict Priority Queueing (SPQ)

- Traffic classes are served based on `priorityLevel` (higher = more preferred).
- Configuration file (`spq.json`) defines each queue’s priority level.

###  Deficit Round Robin (DRR)

- Uses `weight` as the quantum for each traffic class.
- Queues are served in round-robin order, consuming packets if within the deficit budget.

---

##  Simulation Setup

Use a **3-node topology** with a bottleneck link:

[node 0] --- [node 1] --- [node 2]

[Host A] --- [QoS Router] --- [Host B]


The middle node is configured as a QoS-enabled router using either SPQ or DRR.

### 🔍 SPQ Verification

- Two `BulkSend` applications: A and B.
- Application A has higher priority than B.
- A starts *after* B has begun transmitting.
- Expected result: A should preempt B and gain all of the bandwidth due to higher priority.

### 🔍 DRR Verification

- Three queues with quantum in a 3:2:1 ratio.
- Three `BulkSend` applications started simultaneously, each flow should be sorted into one queue.
- Expected result: The throughput distribution follows the weight ratio, 3:2:1.

---

##  Output

Each simulation run generates **four pcap files** to verify traffic behavior before and after the DiffServ router.

###  SPQ Output Files

- `spq-node0-node1-0-0` — Sender traffic before QoS
- `spq-node0-node1-1-0` — Sender traffic before QoS
- `spq-node1-node2-2-0` — Receiver traffic after QoS
- `spq-node1-node2-1-1` — Receiver traffic after QoS

###  DRR Output Files

- `drr-node0-node1-0-0` — Sender traffic before QoS
- `drr-node0-node1-1-0` — Sender traffic before QoS
- `drr-node1-node2-2-0` — Receiver traffic after QoS
- `drr-node1-node2-1-1` — Receiver traffic after QoS

These pcap files allow you to observe:

- **SPQ correctness** — Lower-priority flows yield bandwidth to higher-priority flows as expected, and re-gain bandwidth after the higher-priority flow stops.
- **DRR fairness** — When all three flows are active, they receive bandwidth in proportion to their assigned weights (3:2:1). Once the medium-weight flow stops transmitting, the remaining two flows share the bandwidth in a 3:1 ratio. Finally, when only the highest-weight flow remains, it gains the full available bandwidth.

Use these captured files to generate plots as your primary validation.

## ⚠️ Notes on Packet Classification and Header Requirements

The packet classification logic in this project **relies on the presence of a PPP header** (`ns3::PppHeader`) in every packet. This design simplifies header parsing by ensuring that all packets conform to a predictable structure before network and transport layer fields are accessed.

As a result:

- **You must use `PointToPoint` links** in all simulations. These links attach `PppHeader` by default, enabling filters like `SourceIpAddress`, `DestinationPortNumber`, etc., to work correctly.
- If you switch to other link types (e.g., `Csma`, `Wifi`), the classification may silently fail unless you refractor the `Match()` methods in `FilterElement` to parse the appropriate link-layer headers.
-  If `PppHeader` is not found, `FilterElement::Match()` will log an error and return `false`, meaning the packet may fall back to the default traffic class.

This behavior is consistent across all `FilterElement` types and ensures deterministic filter logic when using `PointToPointHelper`.


## Author

- **Kexin Dai**
- **Tiansi Gu**
