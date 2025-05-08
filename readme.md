# NS-3 Differentiated Services (DiffServ) QoS Project

This project implements **Strict Priority Queueing (SPQ)** and **Deficit Round Robin (DRR)** mechanisms in the [ns-3 network simulator](https://www.nsnam.org/), using a common base class `DiffServ` to simulate differentiated services. It includes both simulation implementations and test topologies.

## Project Structure

Place **all source files and configuration files** inside the `scratch/NS3-DifferentiatedServices/` directory of your `ns-3-dev` workspace. The project includes:

- `diff-serv.cc` / `diff-serv.h`: Base class for DiffServ behavior
- `traffic-class.cc` / `traffic-class.h`: Per-class queue configuration
- `filter.cc`/ `filter.h`, `filter-element.cc`/ `filter-element.h`: Packet classification filters
- `spq.cc`/ `spq.h`, `drr-queue.cc`/ `drr-queue.h`: Implementations of SPQ and DRR
- `main-spq-simulation.cc`: SPQ simulation runner
- `main-drr-simulation.cc`: DRR simulation runner
- `spq.json` / `drr.json`: Queue configuration files

##  How to Compile and Run

Due to ns-3's limitation of supporting only **one `main()` function** at a time in the `scratch` folder, **rename the unused `main-*.cc` to `*.cc.bak`** before running the desired simulation.

### Run DRR Simulation

```bash
# Rename SPQ simulation file to disable it
mv scratch/NS3-DifferentiatedServices/main-spq-simulation.cc scratch/NS3-DifferentiatedServices/main-spq-simulation.cc.bak

# Run DRR simulation
./ns3 run scratch/NS3-DifferentiatedServices/main-drr-simulation --command-template="%s --drrConfig=/path/to/your/drr.json"

```

### Run DRR Simulation

```bash

# Rename DRR simulation file to disable it
mv scratch/NS3-DifferentiatedServices/main-drr-simulation.cc scratch/NS3-DifferentiatedServices/main-drr-simulation.cc.bak
mv scratch/NS3-DifferentiatedServices/main-spq-simulation.cc.bak scratch/NS3-DifferentiatedServices/main-spq-simulation.cc

# Run SPQ simulation
./ns3 run scratch/NS3-DifferentiatedServices/main-spq-simulation --command-template="%s --spqConfig=/path/to/your/spq.json"
```




##  Implemented QoS Mechanisms

###  Strict Priority Queueing (SPQ)

- Traffic classes are served based on `priorityLevel` (higher = more preferred).
- Configuration file (`spq.json`) defines each queue’s priority level.

###  Deficit Round Robin (DRR)

- Uses `weight` as the quantum for each traffic class.
- Queues are served in round-robin order, consuming packets if within the deficit budget.
- Each queue accumulates deficit in proportion to its weight.

---

##  Simulation Setup

Use a **3-node topology** with a bottleneck link:

[Host A] --- [QoS Router] --- [Host B]


The middle node is configured as a QoS-enabled router using either SPQ or DRR.

### 🔍 SPQ Verification

- Two `BulkSend` applications: A and B.
- Application A has higher priority than B.
- A starts *after* B has begun transmitting.
- Expected result: A should preempt B and gain most of the bandwidth due to higher priority.

### 🔍 DRR Verification

- Three queues with quantum weights in a 3:2:1 ratio.
- Three `BulkSend` applications started simultaneously, one per queue.
- Expected result: The throughput distribution follows the weight ratio (e.g., App A > B > C).

---

##  Output

Each simulation run generates **four pcap files** to verify traffic behavior before and after the QoS router.

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

- **SPQ correctness** — Lower-priority flows yield bandwidth to higher-priority flows as expected.
- **DRR fairness** — Flows receive bandwidth proportional to their assigned weights.

Use these captured files to draw plots as your primary validation.

## Author

- **Kexin Dai**
- **Tiansi Gu**
