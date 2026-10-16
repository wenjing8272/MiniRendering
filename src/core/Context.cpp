#include"Context.h"
#include<Logger.h>
#include<array>

namespace MiniRendering {
	
	Context::Context(Context&& other) noexcept
	{
		m_context = std::move(other.m_context);
		m_instance = std::move(other.m_instance);
#ifndef NDEBUG
		m_debugMessenger = std::move(other.m_debugMessenger);
#endif // !defined(NDEBUG)
		m_physicalDevice = std::move(other.m_physicalDevice);
	}

	Context& Context::operator=(Context&& other) noexcept
	{
		if (this != &other)
		{
			m_context = std::move(other.m_context);
			m_instance = std::move(other.m_instance);
#ifndef NDEBUG
			m_debugMessenger = std::move(other.m_debugMessenger);
#endif // !defined(NDEBUG)
			m_physicalDevice = std::move(other.m_physicalDevice);
		}
		return *this;
	}
	vk::raii::Device& Context::getDevice() { return m_device; }
	vk::raii::PhysicalDevice& Context::getPhysicalDevice() { return m_physicalDevice; }
	vk::raii::Instance& Context::getInstance() { return m_instance; }
	Context::~Context()
	{
		MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "destroy vulkan context");
	}
	void Context::initContext(std::vector<const char*>& instanceExtensions)
	{
		this->instanceExtensions = instanceExtensions;
		createInstance();
		pickPhysicalDevice();
	}

	void Context::createInstance()
	{
		constexpr vk::ApplicationInfo appInfo{
			.pApplicationName = "MiniRendering",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "MiniRendering",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = vk::ApiVersion14
		};
		std::vector<const char*>requiredLayer;
		if (enableValidationLayers)
		{
			requiredLayer.assign(validationLayers.begin(), validationLayers.end());
		}
		auto layerProperties = m_context.enumerateInstanceLayerProperties();
		auto unsupportedLayerIt = std::ranges::find_if(requiredLayer,     
			[&layerProperties](auto const& requiredLayer) {				
				return std::ranges::none_of(layerProperties,			
					[requiredLayer](auto const& layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; });
			});
		if (unsupportedLayerIt != requiredLayer.end())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal,"Required layer not supported: " + std::string(*unsupportedLayerIt));    //终止，并打印不支持的扩展
		}
		if (enableValidationLayers)
		{
			instanceExtensions.push_back(vk::EXTDebugUtilsExtensionName);
		}

		auto extensionProperties = m_context.enumerateInstanceExtensionProperties();
		auto unsupportedExtensionIt = std::ranges::find_if(instanceExtensions,
			[&extensionProperties](auto const& requiredExtension) {
				return std::ranges::none_of(extensionProperties,
					[requiredExtension](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; });
			});

		if (unsupportedExtensionIt != instanceExtensions.end())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "Required extension not supported: " + std::string(*unsupportedExtensionIt));
		}
		vk::InstanceCreateInfo instanceCreateInfo{
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(requiredLayer.size()),
			.ppEnabledLayerNames = requiredLayer.data(),
			.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
			.ppEnabledExtensionNames = instanceExtensions.data()
		};
		m_instance = vk::raii::Instance(m_context, instanceCreateInfo);
	}
	void Context::pickPhysicalDevice()
	{
		auto physicalDevices = m_instance.enumeratePhysicalDevices();
		auto deviceIter = std::ranges::find_if(physicalDevices, [&](const auto& pd) {return isDeviceSuitable(pd);});
		if (deviceIter == physicalDevices.end())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "failed to find suitable GPU");
		}
		m_physicalDevice = std::move(*deviceIter);
	}
	bool Context::isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice)
	{
		bool supportsVulkan13 = physicalDevice.getProperties().apiVersion >= vk::ApiVersion13;
		auto queueFamilies = physicalDevice.getQueueFamilyProperties();
		bool supportsGraphics = std::ranges::any_of(queueFamilies,
			[](const auto& queueFamily) {
				return (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) != vk::QueueFlags(0);
			});
		bool supportsCompute = std::ranges::any_of(queueFamilies,
			[](const auto& queueFamily) {
				return (queueFamily.queueFlags & vk::QueueFlagBits::eCompute) != vk::QueueFlags(0);
			});
		bool supportsTransfer = std::ranges::any_of(queueFamilies,
			[](const auto& queueFamily) {
				return (queueFamily.queueFlags & vk::QueueFlagBits::eTransfer) != vk::QueueFlags(0);
			});
		auto availableDeviceExtensions = physicalDevice.enumerateDeviceExtensionProperties();
		bool supportsAllRequiredExtension = std::ranges::all_of(requiredDeviceExtensions,
			[&availableDeviceExtensions](const auto& requiredExtension) {
				return std::ranges::any_of(availableDeviceExtensions,
					[requiredExtension](const auto& availableExtension) {
						return strcmp(requiredExtension, availableExtension.extensionName) == 0;
					});
			});
		auto features = physicalDevice.getFeatures2<vk::PhysicalDeviceFeatures2,
													vk::PhysicalDeviceVulkan11Features,
													vk::PhysicalDeviceVulkan12Features,
													vk::PhysicalDeviceVulkan13Features,
													vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();
		bool supportsRequiredFeatures = features.get<vk::PhysicalDeviceFeatures2>().features.samplerAnisotropy &&
										features.get<vk::PhysicalDeviceVulkan11Features>().shaderDrawParameters &&
										features.get<vk::PhysicalDeviceVulkan12Features>().runtimeDescriptorArray &&
										features.get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
										features.get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>().extendedDynamicState &&
										features.get<vk::PhysicalDeviceVulkan13Features>().synchronization2;
		return supportsVulkan13 && supportsGraphics && supportsCompute && supportsTransfer && supportsAllRequiredExtension && supportsRequiredFeatures;
	}
	void Context::createDevice(const MiniRendering::QueueIndex& queueIndex)
	{
		vk::StructureChain<
			vk::PhysicalDeviceFeatures2,
			vk::PhysicalDeviceVulkan11Features,
			vk::PhysicalDeviceVulkan12Features,
			vk::PhysicalDeviceVulkan13Features,
			vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
			featureChain = {
				{.features = {.samplerAnisotropy = true}},
				{.shaderDrawParameters = true},
				{.runtimeDescriptorArray = true},
				{.synchronization2 = true,.dynamicRendering = true},
				{.extendedDynamicState = true}
		};
		constexpr float queuePriority = 1.0f; //编译期常量
		std::array<vk::DeviceQueueCreateInfo, 4> queueInfos{
			{
				{.queueFamilyIndex = *queueIndex.graphicsQueueIndex,.queueCount = 1,.pQueuePriorities = &queuePriority},
				{.queueFamilyIndex = *queueIndex.presentQueueIndex,.queueCount = 1,.pQueuePriorities = &queuePriority},
				{.queueFamilyIndex = *queueIndex.transferQueueIndex,.queueCount = 1,.pQueuePriorities = &queuePriority},
				{.queueFamilyIndex = *queueIndex.computeQueueIndex,.queueCount = 1,.pQueuePriorities = &queuePriority}
			}
		};
		vk::DeviceCreateInfo deviceCreateInfo{
			.pNext = &featureChain.get<vk::PhysicalDeviceFeatures2>(),
			.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size()),
			.pQueueCreateInfos = queueInfos.data(),
			.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensions.size()),
			.ppEnabledExtensionNames = requiredDeviceExtensions.data()
		};
		m_device = vk::raii::Device(m_physicalDevice, deviceCreateInfo);
	}
} // namespace MiniRendering