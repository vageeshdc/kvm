#ifndef __KVM_COALESCED_MMIO_H__
#define __KVM_COALESCED_MMIO_H__

/*
 * KVM coalesced MMIO
 *
 * Copyright (c) 2008 Bull S.A.S.
 *
 *  Author: Laurent Vivier <Laurent.Vivier@bull.net>
 *
 */

#ifdef CONFIG_KVM_MMIO

#include <linux/list.h>

class kvm_coalesced_mmio_dev {
	struct list_head list;
	kvm_io_device dev;
	struct kvm *kvm;
	struct kvm_coalesced_mmio_zone zone;
	
private:
    static int coalesced_mmio_in_range(gpa_t addr, int len)
    {
	    /* is it in a batchable area ?
	     * (addr,len) is fully included in
	     * (zone->addr, zone->size)
	     */
	    if (len < 0)
		    return 0;
	    if (addr + len < addr)
		    return 0;
	    if (addr < zone.addr)
		    return 0;
	    if (addr + len > zone.addr + zone.size)
		    return 0;
	    return 1;
    }
    
    static int coalesced_mmio_has_room()
    {
	    struct kvm_coalesced_mmio_ring *ring;
	    unsigned avail;

	    /* Are we able to batch it ? */

	    /* last is the first free entry
	     * check if we don't meet the first used entry
	     * there is always one unused entry in the buffer
	     */
	    ring = kvm->coalesced_mmio_ring;
	    avail = (ring->first - ring->last - 1) % KVM_COALESCED_MMIO_MAX;
	    if (avail == 0) {
		    /* full */
		    return 0;
	    }

	    return 1;
    }
    
    
};

int kvm_coalesced_mmio_init(struct kvm *kvm);
void kvm_coalesced_mmio_free(struct kvm *kvm);
int kvm_vm_ioctl_register_coalesced_mmio(struct kvm *kvm,
                                       struct kvm_coalesced_mmio_zone *zone);
int kvm_vm_ioctl_unregister_coalesced_mmio(struct kvm *kvm,
                                         struct kvm_coalesced_mmio_zone *zone);

#else

static inline int kvm_coalesced_mmio_init(struct kvm *kvm) { return 0; }
static inline void kvm_coalesced_mmio_free(struct kvm *kvm) { }

#endif

#endif
