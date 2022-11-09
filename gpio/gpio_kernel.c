#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int index = 44;
	int err = 0;
	int _tsp_irq_num =0;

	if (gpio_is_valid(index)) {
		ret = gpio_request(index, "gpio_44");
		if (ret) {
			pr_err("failed request gpio for test\n");
			return -1;
		}
	}

	//gpio_direction_input(index);
	gpio_direction_output(index, 1);

	index = 45;

	if (gpio_is_valid(index)) {
		ret = gpio_request(index, "gpio_45");
		if (ret) {
			pr_err("failed request gpio for test\n");
			return -1;
		}
	}

	//gpio_direction_input(index);
	gpio_direction_output(index, 0);
	gpio_request(46, "gpio_46");
	gpio_direction_input(46);
	_tsp_irq_num = gpio_to_irq(46);/* get irq */
	printk("new pmu ori gpio is 46, pad irq %d\n",  _tsp_irq_num );
#if 0
	//irq_set_irq_type(_tsp_irq_num, IRQ_TYPE_EDGE_FALLING);
	//irq_set_chained_handler_and_data(_tsp_irq_num, gpio_ts_irq, NULL);
#else
	err = request_irq(_tsp_irq_num, gpio_ts_irq, IRQF_TRIGGER_FALLING, "test_gpio", NULL);
	if (err) {
		dev_err(&pdev->dev, "gpio 46:%d request failed to get irq\n", _tsp_irq_num);
	}else
		dev_err(&pdev->dev, "gpio 46:%d request ok to get irq\n", _tsp_irq_num);
#endif

	pr_info("=========end: gpio kernel test by wei\n");
#endif


	return 0;

}
