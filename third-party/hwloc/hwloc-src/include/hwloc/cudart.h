/*
 * Copyright © 2010-2017 Inria.  All rights reserved.
 * Copyright © 2010-2011 Université Bordeaux
 * Copyright © 2011 Cisco Systems, Inc.  All rights reserved.
 * See COPYING in top-level directory.
 */

/** \file
 * \brief Macros to help interaction between hwloc and the CUDA Runtime API.
 *
 * Applications that use both hwloc and the CUDA Runtime API may want to
 * include this file so as to get topology information for CUDA devices.
 *
 */

#ifndef HWLOC_CUDART_H
#define HWLOC_CUDART_H

#include <hwloc.h>
#include <hwloc/autogen/config.h>
#include <hwloc/helper.h>
#ifdef HWLOC_LINUX_SYS
#include <hwloc/linux.h>
#endif

#include <cuda.h> /* for CUDA_VERSION */
#include <cuda_runtime_api.h>


#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup hwlocality_cudart Interoperability with the CUDA Runtime API
 *
 * This interface offers ways to retrieve topology information about
 * CUDA devices when using the CUDA Runtime API.
 *
 * @{
 */

/** \brief Return the domain, bus and device IDs of the CUDA device whose index is \p idx.
 *
 * Device index \p idx must match the local machine.
 */
static __hwloc_inline int
hwloc_cudart_get_device_pci_ids(hwloc_topology_t topology __hwloc_attribute_unused,
				int idx, int *domain, int *bus, int *dev)
{
  cudaError_t cerr;
  struct cudaDeviceProp prop;

  cerr = cudaGetDeviceProperties(&prop, idx);
  if (cerr) {
    errno = ENOSYS;
    return -1;
  }

#if CUDA_VERSION >= 4000
  *domain = prop.pciDomainID;
#else
  *domain = 0;
#endif

  *bus = prop.pciBusID;
  *dev = prop.pciDeviceID;

  return 0;
}

/** \brief Get the CPU set of logical processors that are physically
 * close to device \p idx.
 *
 * Return the CPU set describing the locality of the CUDA device
 * whose index is \p idx.
 *
 * Topology \p topology and device \p idx must match the local machine.
 * I/O devices detection and the CUDA component are not needed in the topology.
 *
 * The function only returns the locality of the device.
 * If more information about the device is needed, OS objects should
 * be used instead, see hwloc_cudart_get_device_osdev_by_index().
 *
 * This function is currently only implemented in a meaningful way for
 * Linux; other systems will simply get a full cpuset.
 */
static __hwloc_inline int
hwloc_cudart_get_device_cpuset(hwloc_topology_t topology __hwloc_attribute_unused,
			       int idx, hwloc_cpuset_t set)
{
#ifdef HWLOC_LINUX_SYS
  /* If we're on Linux, use the sysfs mechanism to get the local cpus */
#define HWLOC_CUDART_DEVICE_SYSFS_PATH_MAX 128
  char path[HWLOC_CUDART_DEVICE_SYSFS_PATH_MAX];
  FILE *sysfile = NULL;
  int domain, bus, dev;

  if (hwloc_cudart_get_device_pci_ids(topology, idx, &domain, &bus, &dev))
    return -1;

  if (!hwloc_topology_is_thissystem(topology)) {
    errno = EINVAL;
    return -1;
  }

  sprintf(path, "/sys/bus/pci/devices/%04x:%02x:%02x.0/local_cpus", (unsigned) domain, (unsigned) bus, (unsigned) dev);
  sysfile = fopen(path, "r");
  if (!sysfile)
    return -1;

  if (hwloc_linux_parse_cpumap_file(sysfile, set) < 0
      || hwloc_bitmap_iszero(set))
    hwloc_bitmap_copy(set, hwloc_topology_get_complete_cpuset(topology));

  fclose(sysfile);
#else
  /* Non-Linux systems simply get a full cpuset */
  hwloc_bitmap_copy(set, hwloc_topology_get_complete_cpuset(topology));
#endif
  return 0;
}

/** \brief Get the hwloc PCI device object corresponding to the
 * CUDA device whose index is \p idx.
 *
 * Return the PCI device object describing the CUDA device whose
 * index is \p idx. Return NULL if there is none.
 *
 * Topology \p topology and device \p idx must match the local machine.
 * I/O devices detection must be enabled in topology \p topology.
 * The CUDA component is not needed in the topology.
 */
static __hwloc_inline hwloc_obj_t
hwloc_cudart_get_device_pcidev(hwloc_topology_t topology, int idx)
{
  int domain, bus, dev;

  if (hwloc_cudart_get_device_pci_ids(topology, idx, &domain, &bus, &dev))
    return NULL;

  return hwloc_get_pcidev_by_busid(topology, domain, bus, dev, 0);
}

/** \brief Get the hwloc OS device object corresponding to the
 * CUDA device whose index is \p idx.
 *
 * Return the OS device object describing the CUDA device whose
 * index is \p idx. Return NULL if there is none.
 *
 * The topology \p topology does not necessarily have to match the current
 * machine. For instance the topology may be an XML import of a remote host.
 * I/O devices detection and the CUDA component must be enabled in the topology.
 * If not, the locality of the object may still be found using
 * hwloc_cudart_get_device_cpuset().
 *
 * \note The corresponding PCI device object can be obtained by looking
 * at the OS device parent object.
 *
 * \note This function is identical to hwloc_cuda_get_device_osdev_by_index().
 */
static __hwloc_inline hwloc_obj_t
hwloc_cudart_get_device_osdev_by_index(hwloc_topology_t topology, unsigned idx)
{
	hwloc_obj_t osdev = NULL;
	while ((osdev = hwloc_get_next_osdev(topology, osdev)) != NULL) {
		if (HWLOC_OBJ_OSDEV_COPROC == osdev->attr->osdev.type
		    && osdev->name
		    && !strncmp("cuda", osdev->name, 4)
		    && atoi(osdev->name + 4) == (int) idx)
			return osdev;
	}
	return NULL;
}

/** @} */


#ifdef __cplusplus
} /* extern "C" */
#endif


#endif /* HWLOC_CUDART_H */
