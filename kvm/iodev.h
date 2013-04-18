/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __KVM_IODEV_H__
#define __KVM_IODEV_H__

#include <linux/kvm_types.h>
#include <asm/errno.h>


class kvm_io_device{
    
    const struct kvm_io_device_ops *ops;
    
private:
    inline void kvm_iodevice_init(const struct kvm_io_device_ops *ops)
    {
	    this.ops = ops;
    }

    inline int kvm_iodevice_read(gpa_t addr, int l, void *v)
    {
	    return ops->read ? dev->ops->read(&this, addr, l, v) : -EOPNOTSUPP;
    }

    inline int kvm_iodevice_write(gpa_t addr, int l, const void *v)
    {
	    return ops->write ? ops->write(&this, addr, l, v) : -EOPNOTSUPP;
    }

    inline void kvm_iodevice_destructor()
    {
	    if (ops->destructor)
		    ops->destructor(&this);
    }
}


//struct kvm_io_device;

/**
 * kvm_io_device_ops are called under kvm slots_lock.
 * read and write handlers return 0 if the transaction has been handled,
 * or non-zero to have it passed to the next device.
 **/
struct kvm_io_device_ops {
	int (*read)(struct kvm_io_device *this,
		    gpa_t addr,
		    int len,
		    void *val);
	int (*write)(struct kvm_io_device *this,
		     gpa_t addr,
		     int len,
		     const void *val);
	void (*destructor)(struct kvm_io_device *this);
};




#endif /* __KVM_IODEV_H__ */
